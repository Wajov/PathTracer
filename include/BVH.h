#ifndef BVH_H
#define BVH_H

#include <cfloat>
#include <algorithm>
#include <vector>

#include "ConfigHelper.h"
#include "Point.h"
#include "Triangle.h"
#include "AABB.h"
#include "Ray.h"

class BVH {
private:
    AABB aabb;
    std::vector<Triangle> triangles;
    BVH *left, *right;
    static bool compareByX(const Triangle &t0, const Triangle &t1);
    static bool compareByY(const Triangle &t0, const Triangle &t1);
    static bool compareByZ(const Triangle &t0, const Triangle &t1);

public:
    BVH(const std::vector<Triangle> &triangles);
    BVH(const BVH &bvh);
    ~BVH();
    void trace(const Ray &ray, float &t, Point &point) const;
};

#endif