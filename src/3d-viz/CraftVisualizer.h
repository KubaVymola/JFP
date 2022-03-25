#ifndef __CRAFTVISUALIZER_H__
#define __CRAFTVISUALIZER_H__

#include <map>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/mat4x4.hpp"
#include "tinyxml2.h"
#include "renderables/IRenderable.h"
#include "renderables/IRenderTree.h"
#include "renderables/Mesh.h"
#include "Camera.h"
#include "sim/FDMData.h"

class CraftVisualizer : public IRenderTree {
public:
    CraftVisualizer();
    ~CraftVisualizer();
    void Init (const std::string &fileName);
    virtual void AddChild(IRenderTree *child) override;
    virtual void Render(Camera &camera, FDMData fdmData) const;
    virtual void Render(Camera &camera,
                        FDMData fdmData,
                        glm::mat4 rootTransform,
                        glm::mat4 parentTransform) const override;
private:
    void loadObjFile(const std::string &path);
    void processObjNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void processXMLElement(IRenderTree *currentNode, tinyxml2::XMLElement *localRoot);

    std::string _fileName;
    std::string _objFileName;
    std::vector<IRenderTree *> _children;
    std::vector<Mesh> _meshes;
};

#endif