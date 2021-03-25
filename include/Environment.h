#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cmath>

#include <QImage>
#include <QVector3D>

#include "ConstantHelper.h"
#include "Texture.h"

class Environment {
private:
    Texture texture;

public:
    Environment();
    Environment(const Texture &texture);
    ~Environment();
    bool isNull() const;
    QVector3D color(const QVector3D &direction) const;
};

#endif