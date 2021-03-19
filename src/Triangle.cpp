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
    QVector3D o = ray.getOrigin();
    QVector3D d = ray.getDirection();
    QVector3D e1 = p1.getPosition() - p0.getPosition();
    QVector3D e2 = p2.getPosition() - p0.getPosition();
    QVector3D s = o - p0.getPosition();
    QVector3D s1 = QVector3D::crossProduct(s, e1);
    QVector3D s2 = QVector3D::crossProduct(d, e2);

    float w = QVector3D::dotProduct(e1, s2);
    if (w < EPSILON)
        return FLT_MAX;

    float t = QVector3D::dotProduct(e2, s1) / w;
    float u = QVector3D::dotProduct(s, s2) / w;
    float v = QVector3D::dotProduct(d, s1) / w;
    return u >= 0.0f && v >= 0.0f && u + v <= 1.0f && t >= 0.0f ? t : FLT_MAX;
}