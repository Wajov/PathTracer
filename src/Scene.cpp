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
    for (int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
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

QVector3D Scene::shade(const Ray &ray, const float t, const QVector3D &normal, const Material &material, const int depth) const {
    QVector3D position = ray.point(t);
    QVector3D reflection = ray.reflect(normal);

    QVector3D ans(0.0f, 0.0f, 0.0f);
    for (const Mesh &mesh : meshes)
        if (!mesh.getMaterial().getEmissive().isNull()) {
            QVector3D sum(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < SAMPLE_PER_LIGHT; i++) {
                Point sample = mesh.sample();
                QVector3D direction = (sample.getPosition() - position).normalized();

                Ray rayTemp(position, direction);
                float tTemp;
                QVector3D normalTemp;
                Material materialTemp;
                trace(rayTemp, tTemp, normalTemp, materialTemp);

                if ((rayTemp.point(tTemp) - sample.getPosition()).lengthSquared() < EPSILON) {
                    float cosine0 = std::max(QVector3D::dotProduct(normal, direction), 0.0f);
                    float cosine1 = std::max(QVector3D::dotProduct(sample.getNormal(), -direction), 0.0f);
                    sum += mesh.getMaterial().getEmissive() * material.getDiffuse() * cosine0 * cosine1 * mesh.getArea() / ((sample.getPosition() - position).lengthSquared() * PI);
                }
            }
            ans += sum / SAMPLE_PER_LIGHT;
        }

    if (depth < MAX_DEPTH) {
        float theta, phi;
        QVector3D direction, albedo;
        if (randomUniform() <= material.getThreshold()) {
            sampleHemisphere(1.0f, theta, phi);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D tangent = std::fabs(normal.x()) < EPSILON && std::fabs(normal.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(normal.y(), -normal.x(), 0.0f).normalized();
            QVector3D bitangent = QVector3D::crossProduct(tangent, normal);
            direction = (cosine * normal + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent).normalized();
            albedo = material.getDiffuse();
        } else {
            sampleHemisphere(material.getShininess(), theta, phi);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D tangent = std::fabs(reflection.x()) < EPSILON && std::fabs(reflection.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(reflection.y(), -reflection.x(), 0.0f).normalized();
            QVector3D bitangent = QVector3D::crossProduct(tangent, reflection);
            direction = (cosine * reflection + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent).normalized();
            albedo = material.getSpecular() * std::max(QVector3D::dotProduct(normal, direction), 0.0f);
        }

        Ray rayTemp(position, direction);
        float tTemp;
        QVector3D normalTemp;
        Material materialTemp;
        trace(rayTemp, tTemp, normalTemp, materialTemp);

        if (tTemp < FLT_MAX && materialTemp.getEmissive().isNull())
            ans += shade(rayTemp, tTemp, normalTemp, materialTemp, depth + 1) * albedo;
    }

    return ans;
}

QImage Scene::render(const QVector3D &position, const QVector3D &center, const QVector3D &up, const float fovy, const int width, const int height) const {
    QVector3D f = (center - position).normalized();
    QVector3D u = up.normalized();
    QVector3D l = QVector3D::crossProduct(u, f);
    float scale = 2.0f * std::tan(fovy / 360.0f * PI) / (float)height;
    QVector3D du = u * scale;
    QVector3D dl = l * scale;
    QVector3D o = position + f + (du + dl) * (float)height * 0.5f;

    QImage ans(width, height, QImage::Format_RGB32);
    #pragma omp parallel for
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            QVector3D avg(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < SAMPLE_PER_PIXEL; k++) {
                float x, y;
                samplePixel(x, y);
                QVector3D direction = (o - dl * ((float) i + x) - du * ((float) j + y) - position).normalized();

                Ray ray(position, direction);
                float t;
                QVector3D normal;
                Material material;
                trace(ray, t, normal, material);

                if (t == FLT_MAX)
                    continue;
                if (!material.getEmissive().isNull())
                    avg += material.getEmissive();
                else
                    avg += shade(ray, t, normal, material, 0);
            }
            avg /= SAMPLE_PER_PIXEL;
            int r = std::min((int)(avg.x() * 255), 255);
            int g = std::min((int)(avg.y() * 255), 255);
            int b = std::min((int)(avg.z() * 255), 255);

            ans.setPixel(i, j, QColor(r, g, b).rgb());
        }

    return ans;
}