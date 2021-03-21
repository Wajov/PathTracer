#ifndef SCENE_H
#define SCENE_H

#include <cfloat>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QVector3D>
#include <QImage>
#include <QColor>

#include "ConstantHelper.h"
#include "SampleHelper.h"
#include "Point.h"
#include "Triangle.h"
#include "Material.h"
#include "Mesh.h"
#include "Ray.h"

class Scene {
private:
    std::vector<Mesh> meshes;
    void processNode(aiNode *node, const aiScene *scene, std::string &directory);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);
    void trace(const Ray &ray, float &t, QVector3D &normal, Material &material) const;
    QVector3D shade(const Ray &ray) const;

public:
    Scene();
    Scene(const std::string &path);
    ~Scene();
    QImage render(const QVector3D &position, const QVector3D &center, const QVector3D &up, const float fovy, const int width, const int height);
};

#endif