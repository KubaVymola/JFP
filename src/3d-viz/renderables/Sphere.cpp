#include "Sphere.h"

#include <math.h>
#include "../Renderer.h"
#include "../ShaderBank.h"

Sphere::Sphere(glm::dvec3 position, double radius, int stackCount, int sectorCount) {
    _position = position;
    radius = radius;
    
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    
    float stackAngle, sectorAngle;
    float xz, x, y, z;
    
    // Generate vertices
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;

        xz = radius * cosf(stackAngle);
        x = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            z = xz * cosf(sectorAngle);
            y = xz * sinf(sectorAngle);

            Vertex vertex;


            vertex.Position = glm::vec3(x, y, z);
            vertex.Normal = glm::vec3(glm::vec3(x / radius, y / radius, z / radius));

            _vertices.push_back(vertex);
        }
    }

    // Generate indeces
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                _indeces.push_back(k1);
                _indeces.push_back(k2);
                _indeces.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                _indeces.push_back(k1 + 1);
                _indeces.push_back(k2);
                _indeces.push_back(k2 + 1);
            }
        }
    }

    _mesh = Mesh(_vertices, _indeces, "earth");
}

void Sphere::Render(Camera& camera) const {
    Shader * shader = ShaderBank::getInstance().GetShader(ShaderTypes::SIMPLE_OBJ_SHADER);
    
    glm::vec3 posToCamera = _position - Renderer::cameraPos;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, posToCamera);

    shader->Use();
    shader->SetVec3("objectColor", 0.8f, 0.8f, 0.8f);
    shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->SetVec3("lightPos", Renderer::lightPos.x, Renderer::lightPos.y, Renderer::lightPos.z);
    shader->SetVec3("viewPos", Renderer::cameraPos.x, Renderer::cameraPos.y, Renderer::cameraPos.z);

    shader->SetMat4("model", model);
    shader->SetMat4("view", camera.GetViewMatrix());
    shader->SetMat4("projection", Renderer::projection);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    _mesh.Draw();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
