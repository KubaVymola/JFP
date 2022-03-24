#include "CraftVisualizer.h"

#include "tinyxml2.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "renderables/Model3D.h"
#include "Renderer.h"
#include "utils/Constants.h"

CraftVisualizer::CraftVisualizer() {
    
}

CraftVisualizer::~CraftVisualizer() {
    for (std::pair<std::string, Mesh *> meshPair : _meshes) {
        delete meshPair.second;
    }
}

void CraftVisualizer::Init(const std::string& fileName) {
    // TODO parse XML input
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    tinyxml2::XMLElement * root = doc.FirstChildElement("model");

    _fileName = fileName;
    _objFileName = root->FirstChildElement("file")->GetText();

    std::cout << ".obj file: " << _objFileName << std::endl;

    // Parse obj file
    loadModel(_objFileName);

    // Process xml nodes
    std::cout << "All XML elements:" << std::endl;
    processXMLElement(this, root);
}

void CraftVisualizer::processXMLElement(IRenderTree * currentRenderNode, tinyxml2::XMLElement * localRoot) {
    for (tinyxml2::XMLElement * e = localRoot->FirstChildElement("node");
            e != NULL;
            e = e->NextSiblingElement("node")) {
        
        std::string elementName = e->Attribute("name");

        Mesh * mesh = _meshes.at(elementName.c_str());
        Model3D * newModel = new Model3D(mesh);

        // ==== Add offset ====
        tinyxml2::XMLElement * offsetElement = e->FirstChildElement("offset");
        if (offsetElement != NULL) {
            float offsetX = atof(offsetElement->FirstChildElement("x")->GetText());
            float offsetY = atof(offsetElement->FirstChildElement("y")->GetText());
            float offsetZ = atof(offsetElement->FirstChildElement("z")->GetText());
            
            newModel->SetOffset(glm::vec3(offsetX, offsetY, offsetZ));
        }
        // ==== END Add offset ====

        // ==== Add rotations ====
        for (tinyxml2::XMLElement * rotationElement = e->FirstChildElement("rotation");
                rotationElement != NULL;
                rotationElement = rotationElement->NextSiblingElement("rotation")) {
            
            tinyxml2::XMLElement * rootElem = rotationElement->FirstChildElement("root");
            
            float rootX = atof(rootElem->FirstChildElement("x")->GetText());
            float rootY = atof(rootElem->FirstChildElement("y")->GetText());
            float rootZ = atof(rootElem->FirstChildElement("z")->GetText());

            tinyxml2::XMLElement * axisElem = rotationElement->FirstChildElement("axis");
            float axisX = atof(axisElem->FirstChildElement("x")->GetText());
            float axisY = atof(axisElem->FirstChildElement("y")->GetText());
            float axisZ = atof(axisElem->FirstChildElement("z")->GetText());

            tinyxml2::XMLElement * defaultElem = rotationElement->FirstChildElement("default_value");
            float defaultRotation = 0.0f;
            if (defaultElem != NULL) defaultRotation = atof(defaultElem->GetText());

            tinyxml2::XMLElement * propertyNameElem = rotationElement->FirstChildElement("property");
            std::string propertyName;
            if (propertyNameElem != NULL) propertyName = propertyNameElem->GetText();

            RotationStruct newRotation(glm::vec3(rootX, rootY, rootZ),
                                       glm::vec3(axisX, axisY, axisZ),
                                       defaultRotation,
                                       propertyName);

            newModel->AddRotation(newRotation);
        }
        // ==== END Add rotations ====

        currentRenderNode->AddChild(newModel);

        std::cout << elementName << std::endl;
        processXMLElement(newModel ,e);
    }
}

void CraftVisualizer::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR::ASSIMP\n");
        return;
    }

    processObjNode(scene->mRootNode, scene);

    printf("Loaded model with %ld meshes\n", _meshes.size());
}

void CraftVisualizer::processObjNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        Mesh * newMesh = processMesh(mesh, scene);
        _meshes[newMesh->GetName()] = newMesh;
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processObjNode(node->mChildren[i], scene);
    }
}

Mesh * CraftVisualizer::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        vertex.Normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::string meshName = mesh->mName.C_Str();
    std::cout << meshName << std::endl;

    return new Mesh(vertices, indices, meshName);
}

void CraftVisualizer::Render(Camera * camera, FDMData fdmData) const {
    glm::dvec3 rocketPos = glm::dvec3(fdmData.GetValue("latitude_deg") * EARTH_PERIPHERAL_CONSTANT,
                                      fdmData.GetValue("altitude_asl_ft") * FT_TO_M,
                                      fdmData.GetValue("longitude_deg") * EARTH_PERIPHERAL_CONSTANT);

    glm::vec3 rocketPosToCamera = rocketPos - Renderer::cameraPos;
    
    glm::mat4 modelTranslation = glm::mat4(1.0f);
    modelTranslation = glm::translate(modelTranslation, rocketPosToCamera);

    // glm::mat4 quaternionRotation = glm::mat4(1.0f);
    glm::mat4 modelRotation = glm::mat4(1.0f);
    // Change coordinate systems from JSBsim to OpenGL
    modelRotation = glm::mat4(-1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, -1.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f);
    modelRotation = glm::rotate(modelRotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply rotation of model in ECEF frame
    modelRotation *= glm::toMat4(glm::quat(fdmData.GetValue("ecef_q_1"),
                                           fdmData.GetValue("ecef_q_2"),
                                           fdmData.GetValue("ecef_q_3"),
                                           fdmData.GetValue("ecef_q_4")));

    // Move rotation origin to CG
    modelRotation = glm::translate(modelRotation, -glm::vec3(fdmData.GetValue("cg_x_m"),
                                                             -fdmData.GetValue("cg_y_m"),
                                                             fdmData.GetValue("cg_z_m")));

    // Rotations to fit changing axis definitions (between JSBSim and OpenGL)
    modelRotation = glm::rotate(modelRotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelRotation *= glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, -1.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f);


    glm::mat4 model = modelTranslation * modelRotation;

    for (IRenderTree * child : _children) {
        child->Render(camera, fdmData, model, glm::mat4(1.0f));
    }
}


void CraftVisualizer::AddChild(IRenderTree * child) {
    _children.push_back(child);
}

void CraftVisualizer::Render(Camera * camera,
                   FDMData fdmData,
                   glm::mat4 rootTransform,
                   glm::mat4 parentTransform) const {
    this->Render(camera, fdmData);
}