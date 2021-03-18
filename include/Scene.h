#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Scene {
private:
    void processNode(aiNode *node, const aiScene *scene, std::string &directory);
    void processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);

public:
    Scene(const std::string &path);
};

#endif