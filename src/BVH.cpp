#include "BVH.h"

BVH::BVH(const std::vector<Triangle> &triangles) {
    for (const Triangle &triangle : triangles)
        aabb.combine(triangle.aabb());

    if (triangles.size() <= MAX_NUM_TRIANGLE) {
        this->triangles = triangles;
        left = right = nullptr;
    } else {
        float x = aabb.rangeX(), y = aabb.rangeY(), z = aabb.rangeZ();
        if (x >= y && x >= z)
            std::sort(triangles.begin(), triangles.end(), compareByX);
        else if (y >= x && y >= z)
            std::sort(triangles.begin(), triangles.end(), compareByY);
        else
            std::sort(triangles.begin(), triangles.end(), compareByZ);
        auto middle = triangles.begin() + (triangles.size() / 2);
        std::vector<Triangle> leftTriangles(triangles.begin(), middle);
        std::vector<Triangle> rightTriangles(middle, triangles.end());
        left = new BVH(leftTriangles);
        right = new BVH(rightTriangles);
    }
}

BVH::~BVH() {
    delete left;
    delete right;
}

bool BVH::compareByX(const Triangle &t0, const Triangle &t1) {
    return t0.getCenter().x() < t1.getCenter().x();
}

bool BVH::compareByY(const Triangle &t0, const Triangle &t1) {
    return t0.getCenter().y() < t1.getCenter().y();
}

bool BVH::compareByZ(const Triangle &t0, const Triangle &t1) {
    return t0.getCenter().z() < t1.getCenter().z();
}

float BVH::trace(const Ray &ray) const {
    if (!aabb.trace(ray))
        return FLT_MAX;
    if (left != nullptr || right != nullptr)
        return std::min(left->trace(ray), right->trace(ray));

    float ans = FLT_MAX;
    for (const Triangle &triangle : triangles)
        ans = std::min(ans, triangle.trace(ray));
    return ans;
}