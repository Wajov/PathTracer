#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cfloat>

#include <QVector3D>

#include "ConstantHelper.h"
#include "Point.h"
#include "AABB.h"
#include "Ray.h"

class Triangle {
private:
    Point p0, p1, p2;
    QVector3D center;

public:
    Triangle(const Point &p0, const Point &p1, const Point &p2);
    ~Triangle();
    QVector3D getCenter() const;
    AABB aabb() const;
    float trace(const Ray &ray) const;
};

#endif