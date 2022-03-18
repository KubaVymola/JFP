#ifndef _GROUND_H_
#define _GROUND_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Texture.h"

class Ground {
public:
    Ground(const char *texturePath);
    void Draw();
private:
    void initializeBuffers();
    
    GLuint _VAO, _VBO, _EBO;
    Texture *_groundTexture;

    // float _vertices[30] = {
    //      0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
    //     -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,

    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
    //     -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
    // };

    float _vertices[20] = {
         0.5f, 0.0f,  0.5f, 1.0f, 1.0f,
         0.5f, 0.0f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f
    };

    int _indices[6] = {
        0, 1, 3,
        1, 2, 3
    };
};

#endif