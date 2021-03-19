#ifndef AABB_H
#define AABB_H

#include <cfloat>
#include <algorithm>

#include <QVector3D>

#include "ConstantHelper.h"
#include "Point.h"
#include "Ray.h"

class AABB {
private:
    float x0, x1, y0, y1, z0, z1;

public:
    AABB();
    ~AABB();
    void add(const Point &point);
    void combine(const AABB &aabb);
    float rangeX() const;
    float rangeY() const;
    float rangeZ() const;
    bool trace(const Ray &ray) const;
};

#endif