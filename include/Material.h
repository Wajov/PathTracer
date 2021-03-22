#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>

#include <QVector3D>

#include "ConstantHelper.h"

class Material {
private:
    QVector3D diffuse, specular, emissive;
    float shininess;

public:
    Material();
    Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess);
    ~Material();
    QVector3D getEmissive() const;
    QVector3D brdf(const QVector3D &normal, const QVector3D &reflection, const QVector3D &direction) const;
};

#endif