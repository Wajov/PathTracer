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

static void samplePixel(float &x, float &y) {
    x = randomUniform();
    y = randomUniform();
}

static void sampleHemisphere(const float exp, float &theta, float &phi, float &pdf) {
    theta = std::acos(std::pow(randomUniform(), 1.0f / (exp + 1)));
    phi = randomUniform() * PI * 2.0f;
    pdf = std::pow(std::cos(theta), exp) * std::sin(theta) * (exp + 1) / (PI * 2.0f);
}

#endif