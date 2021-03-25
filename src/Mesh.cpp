#include "Mesh.h"

Mesh::Mesh(const std::vector<Triangle> &triangles, const Material &material, const Texture &texture) :
triangles(triangles),
bvh(triangles),
material(material),
texture(texture) {
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

QVector3D Mesh::color(const QVector2D &uv) const {
    return texture.color(uv);
}

void Mesh::trace(const Ray &ray, float &t, Point &point) const {
    return bvh.trace(ray, t, point);
}

Point Mesh::sample() const {
    float t = randomUniform() * area;
    for (int i = 0; i < triangles.size(); i++) {
        t -= areas[i];
        if (t <= 0.0f)
            return triangles[i].sample();
    }
}