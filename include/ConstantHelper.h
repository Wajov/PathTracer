#ifndef CONSTANT_HELPER_H
#define CONSTANT_HELPER_H

#include <cmath>

const float PI = std::acos(-1);
const float EPSILON = 1e-6f;
const int BVH_LIMIT = 5;
const int IMAGE_WIDTH = 720;
const int IMAGE_HEIGHT = 720;
const int SAMPLE_PER_PIXEL = 4;
const int SAMPLE_PER_LIGHT = 4;
const float RUSSIAN_ROULETTE = 0.8f;

#endif