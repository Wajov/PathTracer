#include "Material.h"

Material::Material() {}

Material::Material(const QVector3D &diffuse, const QVector3D &specular, const QVector3D &emissive, const float shininess) :
diffuse(diffuse),
specular(specular),
emissive(emissive),
shininess(shininess) {
    if (specular.isNull())
        threshold = 1.0f + EPSILON;
    else {
        float t = (shininess + 1.0f) * (1.0f - std::pow(0.5f, 1.0f / (shininess + 1.0f)));
        threshold = t / (t + 1.0f);
    }
}

Material::~Material() {}

QVector3D Material::getEmissive() const {
    return emissive;
}

float Material::getThreshold() const {
    return threshold;
}

QVector3D Material::diffuseBRDF() const {
    return diffuse / PI;
}

QVector3D Material::specularBRDF(const QVector3D &reflection, const QVector3D &direction) const {
    float cosine = std::max(QVector3D::dotProduct(reflection, direction), 0.0f);
    return specular * std::pow(cosine, shininess) * (shininess + 1.0f) / (PI * 2.0f);
}

void Material::sample(const QVector3D &normal, const QVector3D &reflection, const QVector3D &color, QVector3D &direction, QVector3D &albedo) const {
    float theta, phi;
    QVector3D tangent, bitangent;
    if (randomUniform() <= threshold) {
        sampleHemisphere(1.0f, theta, phi);
        float cosine = std::cos(theta);
        float sine = std::sin(theta);
        calculateTangentSpace(normal, tangent, bitangent);
        direction = cosine * normal + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
        albedo = diffuse * color;
    } else {
        sampleHemisphere(shininess, theta, phi);
        float cosine = std::cos(theta);
        float sine = std::sin(theta);
        calculateTangentSpace(reflection, tangent, bitangent);
        direction = cosine * reflection + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
        if (QVector3D::dotProduct(normal, direction) < 0.0f)
            direction = cosine * reflection - sine * std::cos(phi) * tangent - sine * std::sin(phi) * bitangent;
        albedo = specular * std::max(QVector3D::dotProduct(normal, direction), 0.0f);
    }
}