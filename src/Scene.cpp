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

QVector3D Scene::shade(const Ray &ray, const Point &point, const Material &material, const QVector3D &color, const int bounce) const {
    QVector3D position = point.getPosition();
    QVector3D normal = point.getNormal();
    QVector3D reflection = ray.reflect(normal);

    QVector3D ans = material.getEmissive();
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
                    QVector3D brdf;
                    if (randomUniform() <= material.getThreshold())
                        brdf = material.diffuseBRDF() * color;
                    else
                        brdf = material.specularBRDF(reflection, direction);
                    sum += mesh.getMaterial().getEmissive() * brdf * cosine0 * cosine1 * mesh.getArea() / (sample.getPosition() - position).lengthSquared();
                }
            }
            ans += sum / (float)SAMPLE_PER_LIGHT;
        }

    if (!environment.isNull()) {
        QVector3D sum(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < SAMPLE_PER_LIGHT; i++) {
            QVector3D direction, albedo;
            material.sample(normal, reflection, color, direction, albedo);

            Ray rayTemp(position, direction);
            float tTemp;
            Point pointTemp;
            Material materialTemp;
            QVector3D colorTemp;
            trace(rayTemp, tTemp, pointTemp, materialTemp, colorTemp);

            if (tTemp == FLT_MAX)
                sum += environment.color(direction) * albedo;
        }
        ans += sum / (float)SAMPLE_PER_LIGHT;
    }

    if (bounce < RUSSIAN_ROULETTE_THRESHOLD || randomUniform() < RUSSIAN_ROULETTE_PROBABILITY) {
        QVector3D direction, albedo;
        material.sample(normal, reflection, color, direction, albedo);

        Ray rayTemp(position, direction);
        float tTemp;
        Point pointTemp;
        Material materialTemp;
        QVector3D colorTemp;
        trace(rayTemp, tTemp, pointTemp, materialTemp, colorTemp);

        if (tTemp < FLT_MAX && materialTemp.getEmissive().isNull())
            ans += shade(rayTemp, pointTemp, materialTemp, colorTemp, bounce + 1) * albedo / (bounce < RUSSIAN_ROULETTE_THRESHOLD ? 1.0f : RUSSIAN_ROULETTE_PROBABILITY);
    }

    return ans;
}

void Scene::sample(const QVector3D &position, const QVector3D &o, const QVector3D &du, const QVector3D &dl, std::vector<std::vector<QVector3D>> &sum) {
    #pragma omp parallel for default(none) shared(position, o, du, dl, sum)
    for (int i = 0; i < sum.size(); i++)
        for (int j = 0; j < sum[i].size(); j++) {
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
                sum[i][j] += environment.color(direction);
            else if (!material.getEmissive().isNull())
                sum[i][j] += material.getEmissive();
            else
                sum[i][j] += shade(ray, point, material, color, 0);
        }
}