#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>
#include "Vertex.h"
#include "IRenderable.h"
#include "Mesh.h"

class Sphere : public IRenderable {
public:
    Sphere(glm::dvec3 position, double radius, int stackCount = 20, int sectorCount = 20);
    virtual void Render(Camera * camera) const override;
private:
    glm::dvec3 _position;
    double radius;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indeces;
    std::vector<glm::vec3> _texCoords;

    Mesh * _mesh;
};

#endif