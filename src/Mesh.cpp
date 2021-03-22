#include "Mesh.h"

Mesh::Mesh(const std::vector<Triangle> &triangles, const Material &material) :
triangles(triangles),
bvh(triangles),
material(material) {
    area = 0.0f;
    for (const Triangle &triangle : this->triangles) {
        float temp = triangle.area();
        areas.push_back(temp);
        area += temp;
    }
}

Mesh::~Mesh() {}

float Mesh::getArea() const {
    return area;
}

Material Mesh::getMaterial() const {
    return material;
}

void Mesh::trace(const Ray &ray, float &t, QVector3D &normal) const {
    return bvh.trace(ray, t, normal);
}

Point Mesh::sample() const {
    float t = randomUniform() * area;
    for (int i = 0; i < triangles.size(); i++) {
        t -= areas[i];
        if (t < 0)
            return triangles[i].sample();
    }
}