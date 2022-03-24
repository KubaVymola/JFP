#include "Line3D.h"

#include "glm/gtc/type_ptr.hpp"
#include "../ShaderBank.h"
#include "../Renderer.h"

Line3D::Line3D(glm::dvec3 posFrom, glm::dvec3 posTo, glm::vec3 colorFrom, glm::vec3 colorTo) {
    _root = posFrom;
    
    _data[0] = 0.0f;
    _data[1] = 0.0f;
    _data[2] = 0.0f;
    _data[3] = colorFrom.x;
    _data[4] = colorFrom.y;
    _data[5] = colorFrom.z;

    _data[6] = posTo.x - posFrom.x;
    _data[7] = posTo.y - posFrom.y;
    _data[8] = posTo.z - posFrom.z;
    _data[9] = colorTo.x;
    _data[10] = colorTo.y;
    _data[11] = colorTo.z;


    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_data), _data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void * )(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Line3D::Render(Camera * camera) const {
    Shader * shader = ShaderBank::getInstance().GetShader(ShaderTypes::SIMPLE_3D_SHADER);

    glm::vec3 linePosToCamera = _root - Renderer::cameraPos;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), linePosToCamera);

    shader->Use();
    shader->SetMat4("model", model);
    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetMat4("projection", Renderer::projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    shader->Unuse();
}
