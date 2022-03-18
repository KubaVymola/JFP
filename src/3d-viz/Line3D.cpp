#include "Line3D.h"

#include "glm/gtc/type_ptr.hpp"
#include "ShaderBank.h"
#include "Renderer.h"

Line3D::Line3D(glm::vec3 posFrom, glm::vec3 posTo, glm::vec3 colorFrom, glm::vec3 colorTo) {
    _data[0] = posFrom.x;
    _data[1] = posFrom.y;
    _data[2] = posFrom.z;
    _data[3] = colorFrom.x;
    _data[4] = colorFrom.y;
    _data[5] = colorFrom.z;

    _data[6] = posTo.x;
    _data[7] = posTo.y;
    _data[8] = posTo.z;
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

    shader->Use();
    shader->SetMat4("view", camera->GetViewMatrix(Renderer::cameraPos));
    shader->SetMat4("projection", Renderer::projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    shader->Unuse();
}
