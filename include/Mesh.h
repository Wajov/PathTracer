#ifndef MESH_H
#define MESH_H

#include <vector>

#include "SampleHelper.h"
#include "Point.h"
#include "Triangle.h"
#include "BVH.h"
#include "Material.h"
#include "Ray.h"

class Mesh {
private:
    std::vector<Triangle> triangles;
    std::vector<float> areas;
    float area;
    BVH bvh;
    Material material;

public:
    Mesh(const std::vector<Triangle> &triangles, const Material &material);
    ~Mesh();
    float getArea() const;
    Material getMaterial() const;
    void trace(const Ray &ray, float &t, QVector3D &normal) const;
    Point sample() const;
};

#endif