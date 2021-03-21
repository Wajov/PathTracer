#include "Material.h"

Material::Material() {}

Material::Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess) {}

Material::~Material() {}

QVector3D Material::getDiffuse() const {
    return diffuse;
}

QVector3D Material::getSpecular() const {
    return specular;
}

QVector3D Material::getEmissive() const {
    return emissive;
}

float Material::getShininess() const {
    return shininess;
}