#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include <cmath>

const float PI = std::acos(-1);
const float EPSILON = 1e-6f;
const float GAMMA = 2.2f;
const int STRATIFY_SIZE = 10;
const int BVH_LIMIT = 1;
const int RUSSIAN_ROULETTE_THRESHOLD = 5;
const float RUSSIAN_ROULETTE_PROBABILITY = 0.8f;
static int SAMPLE_PER_PIXEL = 128;
static int SAMPLE_PER_LIGHT = 1;
static int ITERATION_PER_REFRESH = 10;

#endif