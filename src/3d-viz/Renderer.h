#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include "glm/mat4x4.hpp"
#include "Camera.h"
#include "renderables/IRenderable.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void Init();
    void RegisterRenderable(IRenderable * renderable);
    void Render(Camera &camera) const;

    static glm::mat4 projection;
    static glm::vec3 lightPos;
    static glm::dvec3 cameraPos;
private:
    std::vector<IRenderable *> renderables;
    float _testVertices[9] = {
         0.5f,  0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
         0.0f, -0.0f,  0.0f,
    };
    GLuint _testVAO;
    GLuint _testVBO;
};

#endif