#include "Ray.h"

Ray::Ray(const QVector3D &origin, const QVector3D &direction) :
origin(origin),
direction(direction) {}

Ray::~Ray() {}

QVector3D Ray::getOrigin() const {
    return origin;
}

QVector3D Ray::getDirection() const {
    return direction;
}
