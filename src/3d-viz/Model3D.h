#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <string>
#include "glm/mat4x4.hpp"
#include "IRenderable.h"
#include "IRenderTree.h"
#include "Mesh.h"
#include "Camera.h"
#include "sim/FDMData.h"

struct RotationStruct {
    glm::vec3 rotationRoot;
    glm::vec3 rotationAxis;
    float defaultRotation;
    std::string propertyName;

    RotationStruct(
            glm::vec3 rotationRoot,
            glm::vec3 rotationAxis,
            float defaultRotation,
            const std::string& propertyName)
        : rotationRoot(rotationRoot),
          rotationAxis(rotationAxis),
          defaultRotation(defaultRotation),
          propertyName(propertyName) { }
};

class Model3D : public IRenderable, public IRenderTree {
public:
    Model3D(Mesh * mesh);
    virtual void AddChild(IRenderTree * child) override;
    void SetOffset(glm::vec3 offset);
    void AddRotation(RotationStruct rotation);
    virtual void Render(Camera * camera) const override;
    virtual void Render(Camera * camera,
                        FDMData fdmData,
                        glm::mat4 craftTransform,
                        glm::mat4 parentModelTransform) const override;
private:
    // void loadModel(std::string path);
    // void processNode(aiNode *node, const aiScene *scene);
    // Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // std::vector<Mesh> meshes;
    Mesh * _mesh;
    std::vector<IRenderTree *> _children;
    std::vector<RotationStruct> _rotations;
    glm::vec3 _lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 _offset = glm::vec3(1.0f);
};

#endif