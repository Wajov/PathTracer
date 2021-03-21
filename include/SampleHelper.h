#ifndef SAMPLE_HELPER_H
#define SAMPLE_HELPER_H

#include <random>
#include <ctime>
#include <cmath>

#include "ConstantHelper.h"

static std::default_random_engine engine(std::time(nullptr));
static std::uniform_real_distribution<float> uniform(0.0f, 1.0f);

static float randomUniform() {
    return uniform(engine);
}

static std::pair<float, float> samplePixel() {
    return std::make_pair(randomUniform(), randomUniform());
}

static std::pair<float, float> sampleHemisphere() {
    return std::make_pair(std::acos(randomUniform()), randomUniform() * PI * 2.0f);
}

#endif