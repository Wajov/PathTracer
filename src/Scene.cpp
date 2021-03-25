#include "Scene.h"

Scene::Scene() {}

Scene::Scene(const std::string &meshPath, const std::string &environmentPath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(meshPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    std::string directory = meshPath.substr(0, meshPath.find_last_of('/'));
    if (scene != nullptr)
        processNode(scene->mRootNode, scene, directory);

    if (environmentPath.empty())
        environment = Environment(Texture(QImage()));
    else
        environment = Environment(Texture(QImage(environmentPath.c_str())));
}

Scene::~Scene() {}

void Scene::processNode(const aiNode *node, const aiScene *scene, const std::string &directory) {
    for (int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
    for (int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory);
}

Mesh Scene::processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &directory) const {
    std::vector<Point> points;
    for (int i = 0; i < mesh->mNumVertices; i++) {
        QVector3D position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        QVector3D normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        QVector2D uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        if (mesh->mTextureCoords[0] != nullptr)
            uv = QVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        points.emplace_back(position, normal, uv);
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

    Texture texture = processTexture(materialTemp, directory);

    return Mesh(triangles, material, texture);
}

Texture Scene::processTexture(const aiMaterial *material, const std::string &directory) const {
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString nameTemp;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &nameTemp);
        std::string name = nameTemp.C_Str();
        return Texture(QImage((directory + "/" + name).c_str()));
    } else
        return Texture(QImage());
}

void Scene::trace(const Ray &ray, float &t, Point &point, Material &material, QVector3D &color) const {
    t = FLT_MAX;
    for (const Mesh &mesh : meshes) {
        float tTemp;
        Point pointTemp;
        mesh.trace(ray, tTemp, pointTemp);
        if (tTemp < t) {
            t = tTemp;
            point = pointTemp;
            material = mesh.getMaterial();
            color = mesh.color(point.getUV());
        }
    }
}

QVector3D Scene::shade(const Ray &ray, const Point &point, const Material &material, const QVector3D &color, const int depth) const {
    QVector3D position = point.getPosition();
    QVector3D normal = point.getNormal();
    QVector3D tangent = std::fabs(normal.x()) < EPSILON && std::fabs(normal.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(normal.y(), -normal.x(), 0.0f).normalized();
    QVector3D bitangent = QVector3D::crossProduct(tangent, normal);
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
                Point pointTemp;
                Material materialTemp;
                QVector3D colorTemp;
                trace(rayTemp, tTemp, pointTemp, materialTemp, colorTemp);

                if ((rayTemp.point(tTemp) - sample.getPosition()).lengthSquared() < EPSILON) {
                    float cosine0 = std::max(QVector3D::dotProduct(normal, direction), 0.0f);
                    float cosine1 = std::max(QVector3D::dotProduct(sample.getNormal(), -direction), 0.0f);
                    sum += mesh.getMaterial().getEmissive() * material.getDiffuse() * color * cosine0 * cosine1 * mesh.getArea() / (sample.getPosition() - position).lengthSquared();
                }
            }
            ans += sum / SAMPLE_PER_LIGHT;
        }

    if (!environment.isNull()) {
        QVector3D sum(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < SAMPLE_PER_LIGHT; i++) {
            float theta, phi;
            sampleHemisphere(0.0f, theta, phi);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D direction = cosine * normal + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
            sum += environment.color(direction) * material.getDiffuse() * color * cosine * PI * 2.0f;
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
            direction = cosine * normal + sine * std::cos(phi) * tangent + sine * std::sin(phi) * bitangent;
            albedo = material.getDiffuse() * color;
        } else {
            sampleHemisphere(material.getShininess(), theta, phi);
            float cosine = std::cos(theta);
            float sine = std::sin(theta);
            QVector3D tangentReflection = std::fabs(reflection.x()) < EPSILON && std::fabs(reflection.y()) < EPSILON ? QVector3D(1.0f, 0.0f, 0.0f) : QVector3D(reflection.y(), -reflection.x(), 0.0f).normalized();
            QVector3D bitangentReflection = QVector3D::crossProduct(tangentReflection, reflection);
            direction = cosine * reflection + sine * std::cos(phi) * tangentReflection + sine * std::sin(phi) * bitangentReflection;
            albedo = material.getSpecular() * std::max(QVector3D::dotProduct(normal, direction), 0.0f);
        }

        Ray rayTemp(position, direction);
        float tTemp;
        Point pointTemp;
        Material materialTemp;
        QVector3D colorTemp;
        trace(rayTemp, tTemp, pointTemp, materialTemp, colorTemp);

        if (tTemp == FLT_MAX)
            ans += environment.color(direction) * albedo;
        else if (materialTemp.getEmissive().isNull())
            ans += shade(rayTemp, pointTemp, materialTemp, colorTemp, depth + 1) * albedo;
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
    QVector3D o = position + f + (du * (float)height + dl * (float)width) * 0.5f;

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
                Point point;
                Material material;
                QVector3D color;
                trace(ray, t, point, material, color);

                if (t == FLT_MAX)
                    avg += environment.color(direction);
                if (!material.getEmissive().isNull())
                    avg += material.getEmissive();
                else
                    avg += shade(ray, point, material, color, 0);
            }
            avg /= SAMPLE_PER_PIXEL;

            ans.setPixel(i, j, vectorToColor(avg).rgb());
        }

    return ans;
}