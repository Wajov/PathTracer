#ifndef UTILS_HELPER_H
#define UTILS_HELPER_H

#include <random>
#include <ctime>
#include <cmath>
#include <iostream>

#include <QVector3D>
#include <QColor>

#include "ConstantHelper.h"

static std::default_random_engine engine(std::time(nullptr));
static std::uniform_real_distribution<float> uniform(0.0f, 1.0f);

static QVector3D colorToVector(const QColor &color) {
    float x = (float)color.red() / 255.0f;
    float y = (float)color.green() / 255.0f;
    float z = (float)color.blue() / 255.0f;

    return QVector3D(x, y, z);
}

static QColor vectorToColor(const QVector3D &vector) {
    int r = std::min((int)(std::pow(vector.x(), 1.0f / 2.2f) * 255), 255);
    int g = std::min((int)(std::pow(vector.y(), 1.0f / 2.2f) * 255), 255);
    int b = std::min((int)(std::pow(vector.z(), 1.0f / 2.2f) * 255), 255);

    return QColor(r, g, b);
}

static float randomUniform() {
    return uniform(engine);
}

static void samplePixel(float &x, float &y) {
    int xt = (int)(randomUniform() * STRATIFY_SIZE);
    int yt = (int)(randomUniform() * STRATIFY_SIZE);
    x = ((float)xt + randomUniform()) / STRATIFY_SIZE;
    y = ((float)yt + randomUniform()) / STRATIFY_SIZE;
}

static void sampleHemisphere(const float exp, float &theta, float &phi) {
    theta = std::acos(std::pow(randomUniform(), 1.0f / (exp + 1.0f)));
    phi = randomUniform() * PI * 2.0f;
}

#endif