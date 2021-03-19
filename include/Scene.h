#ifndef SCENE_H
#define SCENE_H

#include <cfloat>
#include <string>
#include <algorithm>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QVector3D>

#include "Point.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Ray.h"

class Scene {
private:
    std::vector<Mesh> meshes;
    void processNode(aiNode *node, const aiScene *scene, std::string &directory);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);

public:
    Scene(const std::string &path);
    ~Scene();
    float trace(const Ray &ray) const;
};

#endif