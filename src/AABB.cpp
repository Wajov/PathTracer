#include "AABB.h"

AABB::AABB() :
x0(FLT_MAX),
x1(-FLT_MAX),
y0(FLT_MAX),
y1(-FLT_MAX),
z0(FLT_MAX),
z1(-FLT_MAX) {}

AABB::~AABB() {}

void AABB::add(const QVector3D &point) {
    x0 = std::min(x0, point.x());
    x1 = std::max(x1, point.x());
    y0 = std::min(y0, point.y());
    y1 = std::max(y1, point.y());
    z0 = std::min(z0, point.z());
    z1 = std::max(z1, point.z());
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

    if (std::fabs(d.x()) > EPSILON) {
        t0 = std::max(t0, (d.x() > 0 ? x0 : x1 - o.x()) / d.x());
        t1 = std::min(t1, (d.x() > 0 ? x1 : x0 - o.x()) / d.x());
    } else if (o.x() < x0 || o.x() > x1)
        return false;

    if (std::fabs(d.y()) > EPSILON) {
        t0 = std::max(t0, (d.y() > 0 ? y0 : y1 - o.y()) / d.y());
        t1 = std::min(t1, (d.y() > 0 ? y1 : y0 - o.y()) / d.y());
    } else if (o.y() < y0 || o.y() > y1)
        return false;

    if (std::fabs(d.z()) > EPSILON) {
        t0 = std::max(t0, (d.z() > 0 ? z0 : z1 - o.z()) / d.z());
        t1 = std::min(t1, (d.z() > 0 ? z1 : z0 - o.z()) / d.z());
    } else if (o.z() < z0 || o.z() > z1)
        return false;

    return t0 >= 0 && t0 <= t1;
}