#include "Point.h"

Point::Point() :
position(0.0f, 0.0f, 0.0f),
normal(0.0f, 0.0f, 0.0f),
uv(0.0f, 0.0f) {}

Point::Point(const QVector3D &position, const QVector3D &normal, const QVector2D &uv) :
position(position),
normal(normal),
uv(uv) {}

Point::~Point() {}

QVector3D Point::getPosition() const {
    return position;
}

QVector3D Point::getNormal() const {
    return normal;
}

QVector2D Point::getUV() const {
    return uv;
}