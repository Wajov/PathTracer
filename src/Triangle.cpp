#include "Triangle.h"

Triangle::Triangle(const Point &p0, const Point &p1, const Point &p2) :
p0(p0),
p1(p1),
p2(p2),
center((p0.getPosition() + p1.getPosition() + p2.getPosition()) / 3.0f) {}

Triangle::~Triangle() {}

QVector3D Triangle::getCenter() const {
    return center;
}

AABB Triangle::aabb() const {
    AABB ans;
    ans.add(p0);
    ans.add(p1);
    ans.add(p2);
    return ans;
}

float Triangle::trace(const Ray &ray) const {
    // TODO
}