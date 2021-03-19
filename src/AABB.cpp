#include "AABB.h"

AABB::AABB() :
x0(FLT_MAX),
x1(-FLT_MAX),
y0(FLT_MAX),
y1(-FLT_MAX),
z0(FLT_MAX),
z1(-FLT_MAX) {}

AABB::~AABB() {}

void AABB::add(const Point &point) {
    QVector3D position = point.getPosition();
    x0 = std::min(x0, position.x());
    x1 = std::max(x1, position.x());
    y0 = std::min(y0, position.y());
    y1 = std::max(y1, position.y());
    z0 = std::min(z0, position.z());
    z1 = std::max(z1, position.z());
}

void AABB::combine(const AABB &aabb) {
    x0 = std::min(x0, aabb.x0);
    x1 = std::max(x1, aabb.x1);
    y0 = std::min(y0, aabb.y0);
    y1 = std::max(y1, aabb.y1);
    z0 = std::min(z0, aabb.z0);
    z1 = std::max(z1, aabb.z1);
}

float AABB::rangeX() const {
    return x1 - x0;
}

float AABB::rangeY() const {
    return y1 - y0;
}

float AABB::rangeZ() const {
    return z1 - z0;
}

bool AABB::trace(const Ray &ray) const {
    QVector3D o = ray.getOrigin(), d = ray.getDirection();
    float t0 = -FLT_MAX, t1 = FLT_MAX;

    if (d.x() > EPSILON) {
        t0 = std::max(t0, (x0 - o.x()) / d.x());
        t1 = std::min(t1, (x1 - o.x()) / d.x());
    } else if (o.x() < x0 || o.x() > x1)
        return false;

    if (d.y() > EPSILON) {
        t0 = std::max(t0, (y0 - o.y()) / d.y());
        t1 = std::min(t1, (y1 - o.y()) / d.y());
    } else if (o.y() < y0 || o.y() > y1)
        return false;

    if (d.z() > EPSILON) {
        t0 = std::max(t0, (z0 - o.z()) / d.z());
        t1 = std::min(t1, (z1 - o.z()) / d.z());
    } else if (o.z() < z0 || o.z() > z1)
        return false;

    return t0 < t1;
}