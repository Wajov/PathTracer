#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>

class Material {
private:
    QVector3D diffuse, specular, emissive;
    float shininess;

public:
    Material();
    Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess);
    ~Material();
    QVector3D getDiffuse() const;
    QVector3D getSpecular() const;
    QVector3D getEmissive() const;
    float getShininess() const;
};

#endif