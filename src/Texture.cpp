#include "Texture.h"

Texture::Texture(const QImage &image) :
image(image) {}

Texture::~Texture() {}

bool Texture::isNull() const {
    return image.isNull();
}

QVector3D Texture::color(const QVector2D &uv) const {
    if (isNull())
        return QVector3D(1.0f, 1.0f, 1.0f);

    float x = uv.x() - (float)(int)uv.x();
    if (x < 0.0f)
        x += 1.0f;
    float y = uv.y() - (float)(int)uv.y();
    if (y < 0.0f)
        y += 1.0f;

    x *= (float)(image.width() - 1);
    y *= (float)(image.height() - 1);
    int x0 = (int)x, y0 = (int)y;
    int x1 = x0 + 1, y1 = y0 + 1;
    float u = x - (float)x0, v = y - (float)y0;

    QVector3D f00 = colorToVector(image.pixelColor(x0, y0));
    QVector3D f01 = colorToVector(image.pixelColor(x0, y1));
    QVector3D f10 = colorToVector(image.pixelColor(x1, y0));
    QVector3D f11 = colorToVector(image.pixelColor(x1, y1));

    return (1.0f - u) * (1.0f - v) * f00 + (1.0f - u) * v * f01 + u * (1.0f - v) * f10 + u * v * f11;
}