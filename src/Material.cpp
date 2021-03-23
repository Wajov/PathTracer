#include "Material.h"

Material::Material() {}

Material::Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess),
threshold(diffuse.length() / (diffuse.length() + specular.length())) {}

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

float Material::getThreshold() const {
    return threshold;
}

QVector3D Material::brdf(const ReflectionType type, const QVector3D &normal, const QVector3D &direction) const {
    float cosine = std::max(QVector3D::dotProduct(normal, direction), 0.0f);
    switch (type) {
        case DIFFUSE:
            return diffuse * cosine / PI;
        case SPECULAR:
            return specular * std::pow(cosine, shininess) * (shininess + 1) / (PI * 2.0f);
        default:
            break;
    }
}