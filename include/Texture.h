#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>
#include <QColor>
#include <QVector3D>
#include <QVector2D>

#include "UtilsHelper.h"

class Texture {
private:
    QImage image;

public:
    Texture(const QImage &image);
    ~Texture();
    bool isNull() const;
    QVector3D color(const QVector2D &uv) const;
};

#endif