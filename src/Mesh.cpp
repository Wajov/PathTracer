#include "Mesh.h"

Mesh::Mesh(const std::vector<Triangle> &triangles, const Material &material) :
bvh(triangles),
material(material) {}

Mesh::~Mesh() {}

Material Mesh::getMaterial() const {
    return material;
}

void Mesh::trace(const Ray &ray, float &t, QVector3D &normal) const {
    return bvh.trace(ray, t, normal);
}