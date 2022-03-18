#include "Model3D.h"

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Mesh.h"
#include "Vertex.h"
#include "ShaderBank.h"
#include "Visualizer3D.h"
#include "utils/Constants.h"

Model3D::Model3D(Mesh * mesh) {
    _mesh = mesh;
}

void Model3D::AddChild(IRenderTree * child) {
    _children.push_back(child);
}

void Model3D::SetOffset(glm::vec3 offset) {
    this->_offset = offset;
}

void Model3D::AddRotation(RotationStruct rotation) {
    std::cout << "Rotation registered" << std::endl;
    std::cout << rotation.rotationRoot.x << ","
                << rotation.rotationAxis.y << ","
                << rotation.defaultRotation << std::endl;
    
    this->_rotations.push_back(rotation);
}

void Model3D::Render(Camera * camera) const {
    // Shader * shader = ShaderBank::getInstance().GetShader(ShaderTypes::SIMPLE_OBJ_SHADER);

    // glm::vec3 rocketPos = glm::vec3(fdmData.latitude_deg * EARTH_PERIPHERAL_CONSTANT,
    //                                 fdmData.altitude_asl_ft,
    //                                 fdmData.longitude_deg * EARTH_PERIPHERAL_CONSTANT);

    // glm::mat4 modelPosition = glm::mat4(1.0f);
    // modelPosition = glm::translate(modelPosition, rocketPos);

    // glm::mat4 modelRotation = glm::mat4(1.0f);
    // modelRotation = glm::rotate(modelRotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // glm::mat4 quaternionRotation = glm::mat4(1.0f);
    // // Change coordinate systems from JSBsim to OpenGL
    // quaternionRotation = glm::mat4(-1.0f, 0.0f, 0.0f, 0.0f,
    //                                 0.0f, 0.0f, -1.0f, 0.0f,
    //                                 0.0f, 1.0f, 0.0f, 0.0f,
    //                                 0.0f, 0.0f, 0.0f, 1.0f);
    // quaternionRotation *= glm::toMat4(glm::quat(fdmData.q_1, fdmData.q_2, fdmData.q_3, fdmData.q_4));

    
    // glm::mat4 modelScale = glm::mat4(1.0);

    // glm::mat4 model = modelPosition * modelScale * quaternionRotation * modelRotation;

    // shader->Use();
    // shader->SetVec3("objectColor", 0.8f, 0.8f, 0.8f);
    // shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    // shader->SetVec3("lightPos", _lightPos.x, _lightPos.y, _lightPos.z);
    // shader->SetVec3("viewPos", Renderer::cameraPos.x, Renderer::cameraPos.y, Renderer::cameraPos.z);

    // shader->SetMat4("model", model);
    // shader->SetMat4("view", camera->GetViewMatrix(Renderer::cameraPos));
    // shader->SetMat4("projection", Renderer::projection);

    // _mesh->Draw();

    // this->Render(camera, fdmData, glm::mat4(1.0f), glm::mat4(1.0f));
}

void Model3D::Render(Camera * camera,
                     FDMData fdmData,
                     glm::mat4 craftTransform,
                     glm::mat4 parentModelTransform) const {
    Shader * shader = ShaderBank::getInstance().GetShader(ShaderTypes::SIMPLE_OBJ_SHADER);

    glm::mat4 currentTransform = glm::mat4(1.0f);
    currentTransform *= parentModelTransform;

    // Translate by offset
    currentTransform = glm::translate(currentTransform, _offset);
    
    // Apply all rotations (first registered is applied first)
    for (auto it = _rotations.rbegin();
            it != _rotations.rend();
            ++it) {
        const RotationStruct rotation = *it;

        currentTransform = glm::translate(currentTransform, rotation.rotationRoot);


        currentTransform = glm::rotate(currentTransform,
                                       (float)fdmData.GetValue(rotation.propertyName), 
                                       -rotation.rotationAxis);

        // }
        // if (i == 1) {
        //     currentTransform = glm::rotate(currentTransform, fdmData.engine_pitch_rad, -rotation.rotationAxis);
        // }
        // } else {
        //     currentTransform = glm::rotate(currentTransform, glm::radians(rotation.defaultRotation), rotation.rotationAxis);
        // }

        currentTransform = glm::translate(currentTransform, -rotation.rotationRoot);
    }


    glm::mat4 model = craftTransform * currentTransform;

    shader->Use();
    shader->SetVec3("objectColor", 0.8f, 0.8f, 0.8f);
    shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->SetVec3("lightPos", _lightPos.x, _lightPos.y, _lightPos.z);
    shader->SetVec3("viewPos", Renderer::cameraPos.x, Renderer::cameraPos.y, Renderer::cameraPos.z);

    shader->SetMat4("model", model);
    shader->SetMat4("view", camera->GetViewMatrix(Renderer::cameraPos));
    shader->SetMat4("projection", Renderer::projection);

    _mesh->Draw();

    for (IRenderTree * child : _children) {
        child->Render(camera, fdmData, craftTransform, currentTransform);
    }
}