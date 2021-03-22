#include "Scene.h"

Scene::Scene() {}

Scene::Scene(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);
}

Scene::~Scene() {}

void Scene::processNode(aiNode *node, const aiScene *scene, std::string &directory) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
        if (!meshes[i].getMaterial().getEmissive().isNull())
            lights.push_back(meshes[i]);
    }
    for (int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory);
}

Mesh Scene::processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory) const {
    std::vector<Point> points;
    for (int i = 0; i < mesh->mNumVertices; i++) {
        QVector3D position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        QVector3D normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        points.emplace_back(position, normal);
    }
    std::vector<Triangle> triangles;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        Point p0 = points[mesh->mFaces[i].mIndices[0]];
        Point p1 = points[mesh->mFaces[i].mIndices[1]];
        Point p2 = points[mesh->mFaces[i].mIndices[2]];
        triangles.emplace_back(p0, p1, p2);
    }

    aiMaterial *materialTemp = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D diffuseTemp, specularTemp, emissiveTemp;
    float shininess;
    materialTemp->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseTemp);
    materialTemp->Get(AI_MATKEY_COLOR_SPECULAR, specularTemp);
    materialTemp->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveTemp);
    materialTemp->Get(AI_MATKEY_SHININESS, shininess);
    QVector3D diffuse(diffuseTemp.r, diffuseTemp.g, diffuseTemp.b);
    QVector3D specular(specularTemp.r, specularTemp.g, specularTemp.b);
    QVector3D emissive(emissiveTemp.r, emissiveTemp.g, emissiveTemp.b);
    Material material(diffuse, specular, emissive, shininess);

    return Mesh(triangles, material);
}

void Scene::trace(const Ray &ray, float &t, QVector3D &normal, Material &material) const {
    t = FLT_MAX;
    for (const Mesh &mesh : meshes) {
        float tTemp;
        QVector3D normalTemp;
        mesh.trace(ray, tTemp, normalTemp);
        if (tTemp < t) {
            t = tTemp;
            normal = normalTemp;
            material = mesh.getMaterial();
        }
    }
}

QVector3D Scene::shade(const Ray &ray) const {
    float t;
    QVector3D normal;
    Material material;
    trace(ray, t, normal, material);

    if (t == FLT_MAX)
        return QVector3D(0.0f, 0.0f, 0.0f);

    QVector3D position = ray.point(t);
    QVector3D reflection = ray.reflect(normal);

    QVector3D ans = material.getEmissive() / (PI * 4.0f);
    for (const Mesh &light : lights) {
        QVector3D sum(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < SAMPLE_PER_LIGHT; i++) {
            Point sample = light.sample();
            QVector3D direction = (sample.getPosition() - position).normalized();

            Ray rayTemp(position, direction);
            float tTemp;
            QVector3D normalTemp;
            Material materialTemp;
            trace(rayTemp, tTemp, normalTemp, materialTemp);

            if ((rayTemp.point(tTemp) - sample.getPosition()).lengthSquared() < EPSILON) {
                QVector3D brdf = randomUniform() <= material.getThreshold() ? material.diffuseBRDF(normal, direction) : material.specularBRDF(reflection, direction);
                float cosine0 = QVector3D::dotProduct(normal, direction);
                float cosine1 = QVector3D::dotProduct(sample.getNormal(), -direction);
                sum += light.getMaterial().getEmissive() * brdf * cosine0 * cosine1 / (sample.getPosition() - position).lengthSquared() * light.getArea();
            }
        }
        ans += sum / SAMPLE_PER_LIGHT;
    }

    if (randomUniform() < RUSSIAN_ROULETTE) {
        float theta, phi, pdf;
        QVector3D direction, brdf;
        if (randomUniform() <= material.getThreshold()) {
            sampleHemisphere(1.0f, theta, phi, pdf);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D tangent = std::fabs(normal.x()) < EPSILON && std::fabs(normal.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(normal.y(), -normal.x(), 0.0f).normalized();
            QVector3D bitangent = QVector3D::crossProduct(tangent, normal);
            direction = cosine * normal + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
            brdf = material.diffuseBRDF(normal, direction);
        } else {
            sampleHemisphere(material.getShininess(), theta, phi, pdf);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D tangent = std::fabs(reflection.x()) < EPSILON && std::fabs(reflection.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(reflection.y(), -reflection.x(), 0.0f).normalized();
            QVector3D bitangent = QVector3D::crossProduct(tangent, reflection);
            direction = cosine * reflection + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
            brdf = material.specularBRDF(reflection, direction);
        }

        float cosine = QVector3D::dotProduct(normal, direction);
        ans += shade(Ray(position, direction)) * brdf * cosine / (pdf * RUSSIAN_ROULETTE);
    }

    return ans;
}

QImage Scene::render(const QVector3D &position, const QVector3D &center, const QVector3D &up, const float fovy, const int width, const int height) const {
    QVector3D f = (center - position).normalized();
    QVector3D u = up.normalized();
    QVector3D l = QVector3D::crossProduct(u, f);
    float t = 2.0f * std::tan(fovy / 360.0f * PI) / (float)height;
    QVector3D du = u * t;
    QVector3D dl = l * t;
    QVector3D o = position + f + (du + dl) * (float)height * 0.5f;

    QImage ans(width, height, QImage::Format_RGB32);
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            QVector3D avg(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < SAMPLE_PER_PIXEL; k++) {
                float x, y;
                samplePixel(x, y);
                QVector3D direction = (o - dl * ((float) i + x) - du * ((float) j + y) - position).normalized();
                avg += shade(Ray(position, direction));
            }
            avg /= SAMPLE_PER_PIXEL;
            int r = std::min((int)(avg.x() * 255), 255);
            int g = std::min((int)(avg.y() * 255), 255);
            int b = std::min((int)(avg.z() * 255), 255);

            ans.setPixel(i, j, QColor(r, g, b).rgb());
        }

    return ans;
}