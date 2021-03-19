#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Triangle.h"
#include "BVH.h"
#include "Ray.h"

class Mesh {
private:
    QVector3D diffuse, specular, emissive;
    float shininess;
    BVH bvh;

public:
    Mesh(const std::vector<Triangle> &triangles, const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess);
    ~Mesh();
    float trace(const Ray &ray) const;
};

#endif