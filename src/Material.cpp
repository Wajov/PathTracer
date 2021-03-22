#include "Material.h"

Material::Material() {}

Material::Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess) {}

Material::~Material() {}

QVector3D Material::getEmissive() const {
    return emissive;
}

QVector3D Material::brdf(const QVector3D &normal, const QVector3D &reflection, const QVector3D &direction) const {
    float cosine0 = QVector3D::dotProduct(normal, direction);
    float cosine1 = QVector3D::dotProduct(reflection, direction);
    return diffuse * cosine0 / PI + specular * std::pow(cosine1, shininess) * (shininess + 1) / (PI * 2.0f);
}