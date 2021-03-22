#include "Material.h"

Material::Material() {}

Material::Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess),
threshold(diffuse.length() / (diffuse.length() + specular.length())) {}

Material::~Material() {}

QVector3D Material::getEmissive() const {
    return emissive;
}

float Material::getShininess() const {
    return shininess;
}

float Material::getThreshold() const {
    return threshold;
}

QVector3D Material::diffuseBRDF(const QVector3D &normal, const QVector3D &direction) const {
    float cosine = std::max(QVector3D::dotProduct(normal, direction), 0.0f);
    return diffuse * cosine / PI;
}

QVector3D Material::specularBRDF(const QVector3D &reflection, const QVector3D &direction) const {
    float cosine = std::max(QVector3D::dotProduct(reflection, direction), 0.0f);
    return specular * std::pow(cosine, shininess) * (shininess + 1) / (PI * 2.0f);
}