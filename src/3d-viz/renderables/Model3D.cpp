#include "Model3D.h"

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Mesh.h"
#include "Vertex.h"
#include "../ShaderBank.h"
#include "../Visualizer3D.h"
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

void Model3D::Render(Camera * camera,
                     FDMData fdmData,
                     glm::mat4 craftTransform,
                     glm::mat4 parentModelTransform) const {
    Shader * shader = ShaderBank::getInstance().GetShader(ShaderTypes::SIMPLE_OBJ_SHADER);

    glm::mat4 currentTransform = glm::mat4(1.0f);
    currentTransform *= parentModelTransform;

    // Translate by offset
    currentTransform = glm::translate(currentTransform, _offset);
    
    // ==== Apply all rotations (first registered is applied first) ====
    for (auto it = _rotations.rbegin();
            it != _rotations.rend();
            ++it) {
        const RotationStruct rotation = *it;

        currentTransform = glm::translate(currentTransform, rotation.rotationRoot);

        currentTransform = glm::rotate(currentTransform,
                                       (float)fdmData.GetValue(rotation.propertyName), 
                                       -rotation.rotationAxis);

        currentTransform = glm::translate(currentTransform, -rotation.rotationRoot);
    }
    // ==== END rotations ====


    glm::mat4 model = craftTransform * currentTransform;

    shader->Use();
    shader->SetVec3("objectColor", 0.8f, 0.8f, 0.8f);
    shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->SetVec3("lightPos", Renderer::lightPos.x, Renderer::lightPos.y, Renderer::lightPos.z);
    shader->SetVec3("viewPos", Renderer::cameraPos.x, Renderer::cameraPos.y, Renderer::cameraPos.z);

    shader->SetMat4("model", model);
    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetMat4("projection", Renderer::projection);

    _mesh->Draw();

    for (IRenderTree * child : _children) {
        child->Render(camera, fdmData, craftTransform, currentTransform);
    }
}