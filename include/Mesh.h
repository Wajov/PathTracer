#ifndef MESH_H
#define MESH_H

#include <vector>

#include "UtilsHelper.h"
#include "Point.h"
#include "Triangle.h"
#include "BVH.h"
#include "Material.h"
#include "Texture.h"
#include "Ray.h"

class Mesh {
private:
    std::vector<Triangle> triangles;
    std::vector<float> areas;
    float area;
    BVH bvh;
    Material material;
    Texture texture;

public:
    Mesh(const std::vector<Triangle> &triangles, const Material &material, const Texture &texture);
    ~Mesh();
    float getArea() const;
    Material getMaterial() const;
    QVector3D color(const QVector2D &uv) const;
    void trace(const Ray &ray, float &t, Point &point) const;
    Point sample() const;
};

#endif