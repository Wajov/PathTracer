#include "Environment.h"

Environment::Environment() :
texture(QImage()) {}

Environment::Environment(const Texture &texture) :
texture(texture) {}

Environment::~Environment() {}

bool Environment::isNull() const {
    return texture.isNull();
}

QVector3D Environment::color(const QVector3D &direction) const {
    if (texture.isNull())
        return QVector3D(0.0f, 0.0f, 0.0f);

    float u = 0.5f - std::atan2(-direction.z(), -direction.x()) / (PI * 2.0f);
    float v = 0.5f - std::asin(direction.y()) / PI;

    return texture.color(QVector2D(u, v));
}