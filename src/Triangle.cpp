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

float Triangle::area() const {
    return 0.5f * QVector3D::crossProduct(p1.getPosition() - p0.getPosition(), p2.getPosition() - p0.getPosition()).length();
}

AABB Triangle::aabb() const {
    AABB ans;
    ans.add(p0.getPosition());
    ans.add(p1.getPosition());
    ans.add(p2.getPosition());
    return ans;
}

void Triangle::trace(const Ray &ray, float &t, Point &point) const {
    QVector3D o = ray.getOrigin();
    QVector3D d = ray.getDirection();
    QVector3D e1 = p1.getPosition() - p0.getPosition();
    QVector3D e2 = p2.getPosition() - p0.getPosition();
    QVector3D s = o - p0.getPosition();
    QVector3D s1 = QVector3D::crossProduct(s, e1);
    QVector3D s2 = QVector3D::crossProduct(d, e2);

    float w = QVector3D::dotProduct(e1, s2);
    if (w < EPSILON)
        t = FLT_MAX;
    else {
        float tTemp = QVector3D::dotProduct(e2, s1) / w;
        float u = QVector3D::dotProduct(s, s2) / w;
        float v = QVector3D::dotProduct(d, s1) / w;
        if (tTemp > EPSILON && u >= 0.0f && v >= 0.0f && u + v <= 1.0f) {
            t = tTemp;
            QVector3D position = ray.point(t);
            QVector3D normal = ((1.0f - u - v) * p0.getNormal() + u * p1.getNormal() + v * p2.getNormal()).normalized();
            QVector2D uv = (1.0f - u - v) * p0.getUV() + u * p1.getUV() + v * p2.getUV();
            point = Point(position, normal, uv);
        } else
            t = FLT_MAX;
    }
}

Point Triangle::sample() const {
    float u = std::sqrt(randomUniform()), v = randomUniform();

    QVector3D position = (1.0f - u) * p0.getPosition() + u * (1.0f - v) * p1.getPosition() + u * v * p2.getPosition();
    QVector3D normal = ((1.0f - u) * p0.getNormal() + u * (1.0f - v) * p1.getNormal() + u * v * p2.getNormal()).normalized();
    QVector2D uv = (1.0f - u) * p0.getUV() + u * (1.0f - v) * p1.getUV() + u * v * p2.getUV();
    return Point(position, normal, uv);
}