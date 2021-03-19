#include "Point.h"

Point::Point(const QVector3D &position, const QVector3D &normal) :
position(position),
normal(normal) {}

Point::~Point() {}

QVector3D Point::getPosition() const {
    return position;
}

QVector3D Point::getNormal() const {
    return normal;
}