#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Triangle.h"
#include "BVH.h"
#include "Material.h"
#include "Ray.h"

class Mesh {
private:
    BVH bvh;
    Material material;

public:
    Mesh(const std::vector<Triangle> &triangles, const Material &material);
    ~Mesh();
    Material getMaterial() const;
    void trace(const Ray &ray, float &t, QVector3D &normal) const;
};

#endif