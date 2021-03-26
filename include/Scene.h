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
#include <QString>
#include <QVector3D>
#include <QVector2D>
#include <QImage>
#include <QColor>
#include <omp.h>

#include "ConfigHelper.h"
#include "UtilsHelper.h"
#include "Point.h"
#include "Triangle.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Environment.h"
#include "Ray.h"

class Scene {
private:
    std::vector<Mesh> meshes;
    Environment environment;
    void processNode(const aiNode *node, const aiScene *scene, const std::string &directory);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &directory) const;
    Texture processTexture(const aiMaterial *material, const std::string &directory) const;
    void trace(const Ray &ray, float &t, Point &point, Material &material, QVector3D &color) const;
    QVector3D shade(const Ray &ray, const Point &point, const Material &material, const QVector3D &color, const int bounce) const;

public:
    Scene();
    Scene(const std::string &meshPath, const std::string &environmentPath = "");
    ~Scene();
    void sample(const QVector3D &position, const QVector3D &o, const QVector3D &du, const QVector3D &dl, std::vector<std::vector<QVector3D>> &sum);
};

#endif