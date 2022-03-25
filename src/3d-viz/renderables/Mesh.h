#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <string>

#include "../Shader.h"
#include "Vertex.h"

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string& name);
    void Draw() const;
    std::string GetName() const { return _name; }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
private:
    unsigned int _VAO, _VBO, _EBO;
    std::string _name;

    void setupMesh();
};

#endif