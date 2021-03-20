#include "Mesh.h"

Mesh::Mesh(const std::vector<Triangle> &triangles, const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
bvh(triangles),
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess) {}

Mesh::~Mesh() {}

QVector3D Mesh::getDiffuse() const {
    return diffuse;
}

QVector3D Mesh::getSpecular() const {
    return specular;
}

QVector3D Mesh::getEmissive() const {
    return emissive;
}

float Mesh::getShininess() const {
    return shininess;
}

float Mesh::trace(const Ray &ray) const {
    return bvh.trace(ray);
}