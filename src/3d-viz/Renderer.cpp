#include "Renderer.h"

#include "ShaderBank.h"
#include "Visualizer3D.h"
#include "Line3D.h"
#include "utils/Constants.h"

glm::dmat4 Renderer::projection;
glm::dvec3 Renderer::cameraPos;

Renderer::Renderer() {
    // ==== Cartesian-coordinates indicator (at origin) ====
    // X-coord
    renderables.push_back(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(1.0f, 0.0f, 0.0f),
                                     glm::vec3(1.0f, 0.0f, 0.0f),
                                     glm::vec3(1.0f, 0.0f, 0.0f)));
    // Y-coord                       
    renderables.push_back(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f)));

    // Z-coord
    renderables.push_back(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f)));

    // ==== END Cartesian-coordinates indicator ====
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

    Renderer::projection = glm::perspective(glm::radians(45.0),
                                            (double)Visualizer3D::windowWidth / Visualizer3D::windowHeight,
                                            0.1,
                                            300.0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (IRenderable * renderable : renderables) {
        renderable->Render(camera);
    }
}
