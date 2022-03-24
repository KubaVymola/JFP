#ifndef __LINE3D_H__
#define __LINE3D_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "IRenderable.h"
#include "../Camera.h"
#include "sim/FDMData.h"
#include "glm/vec3.hpp"

class Line3D : public IRenderable {
public:
    Line3D(glm::dvec3 posFrom, glm::dvec3 posTo, glm::vec3 colorFrom, glm::vec3 colorTo);
    void Render(Camera * camera) const;
private:
    float _data[12];
    glm::dvec3 _root;

    GLuint _VAO;
    GLuint _VBO;
};

#endif