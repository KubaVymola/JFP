#include "Renderer.h"

#include "glm/vec3.hpp"
#include "ShaderBank.h"
#include "Visualizer3D.h"
#include "renderables/Line3D.h"
#include "utils/Constants.h"

glm::mat4 Renderer::projection;
glm::vec3 Renderer::lightPos = glm::vec3(5.0f, 50.0f, 5.0f);
glm::dvec3 Renderer::cameraPos;

Renderer::Renderer() {
    
}

Renderer::~Renderer() {

}

void Renderer::Init() {
    glGenVertexArrays(1, &_testVAO);
    glGenBuffers(1, &_testVBO);

    glBindVertexArray(_testVAO);

    glBindBuffer(GL_ARRAY_BUFFER, _testVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_testVertices), _testVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void Renderer::RegisterRenderable(IRenderable * renderable) {
    renderables.push_back(renderable);
}

void Renderer::Render(Camera * camera) const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer::projection = glm::perspective((float)glm::radians(45.0),
                                            (float)Visualizer3D::windowWidth / Visualizer3D::windowHeight,
                                            0.1f,
                                            300.0f);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (IRenderable * renderable : renderables) {
        renderable->Render(camera);
    }
}
