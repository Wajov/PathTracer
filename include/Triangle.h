#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cfloat>

#include <QVector3D>

#include "ConstantHelper.h"
#include "SampleHelper.h"
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
    float area() const;
    AABB aabb() const;
    void trace(const Ray &ray, float &t, QVector3D &normal) const;
    Point sample() const;
};

#endif