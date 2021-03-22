#ifndef CONSTANT_HELPER_H
#define CONSTANT_HELPER_H

#include <cmath>

const float PI = std::acos(-1);
const float EPSILON = 1e-10f;
const int BVH_LIMIT = 1;
const int IMAGE_WIDTH = 720;
const int IMAGE_HEIGHT = 720;
const int SAMPLE_PER_PIXEL = 1;
const int SAMPLE_PER_LIGHT = 1;
const float RUSSIAN_ROULETTE = 0.9f;

#endif