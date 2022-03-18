#ifndef __CRAFTVISUALIZER_H__
#define __CRAFTVISUALIZER_H__

#include <map>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/mat4x4.hpp"
#include "tinyxml2.h"
#include "IRenderable.h"
#include "IRenderTree.h"
#include "Mesh.h"
#include "Camera.h"
#include "sim/FDMData.h"

class CraftVisualizer : public IRenderTree {
public:
    CraftVisualizer();
    ~CraftVisualizer();
    void Init (const std::string& fileName);
    virtual void AddChild(IRenderTree * child) override;
    virtual void Render(Camera * camera, FDMData) const;
    virtual void Render(Camera * camera,
                        FDMData fdmData,
                        glm::mat4 rootTransform,
                        glm::mat4 parentTransform) const override;
private:
    void loadModel(const std::string& path);
    void processObjNode(aiNode * node, const aiScene * scene);
    Mesh * processMesh(aiMesh * mesh, const aiScene * scene);
    void processXMLElement(IRenderTree * currentNode, tinyxml2::XMLElement * localRoot);

    std::string _fileName;
    std::string _objFileName;
    std::vector<IRenderTree *> _children;
    std::map<std::string, Mesh *> _meshes;
};

#endif