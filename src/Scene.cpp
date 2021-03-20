#include "Scene.h"

Scene::Scene() {}

Scene::Scene(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);
}

Scene::~Scene() {}

void Scene::processNode(aiNode *node, const aiScene *scene, std::string &directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory);
}

Mesh Scene::processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory) {
    std::vector<Point> points;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        QVector3D position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        QVector3D normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        points.emplace_back(position, normal);
    }
    std::vector<Triangle> triangles;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        Point p0 = points[mesh->mFaces[i].mIndices[0]];
        Point p1 = points[mesh->mFaces[i].mIndices[1]];
        Point p2 = points[mesh->mFaces[i].mIndices[2]];
        triangles.emplace_back(p0, p1, p2);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D diffuseColor, specularColor, emissiveColor;
    float shininess;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
    material->Get(AI_MATKEY_SHININESS, shininess);
    QVector3D diffuse(diffuseColor.r, diffuseColor.g, diffuseColor.b);
    QVector3D specular(specularColor.r, specularColor.g, specularColor.b);
    QVector3D emissive(emissiveColor.r, emissiveColor.g, emissiveColor.b);

    return Mesh(triangles, diffuse, specular, emissive, shininess);
}

QColor Scene::trace(const Ray &ray) const {
    float t = FLT_MAX;
    int index = -1;
    for (int i = 0; i < meshes.size(); i++) {
        float tmp = meshes[i].trace(ray);
        if (tmp < t) {
            t = tmp;
            index = i;
        }
    }

    return index >= 0 ? shade(ray.point(t), index) : QColor(0, 0, 0);
}

QColor Scene::shade(const QVector3D &point, const int index) const {
    QVector3D color = meshes[index].getDiffuse() + meshes[index].getSpecular();
    color.setX(std::min(color.x(), 1.0f));
    color.setY(std::min(color.y(), 1.0f));
    color.setZ(std::min(color.z(), 1.0f));

    return QColor((int)(color.x() * 255), (int)(color.y() * 255), (int)(color.z() * 255));
}

QImage Scene::render(const QVector3D &position, const QVector3D &center, const QVector3D &up, const float fovy, const int width, const int height) {
    QVector3D f = (center - position).normalized();
    QVector3D u = up.normalized();
    QVector3D l = QVector3D::crossProduct(u, f);
    float k = std::tan(fovy / 180.0f * PI) / (float)height;
    QVector3D du = u * k;
    QVector3D dl = l * k;
    QVector3D o = position + f + du * (float)height + dl * (float)width * 0.5f;

    QImage ans(width, height, QImage::Format_RGB32);
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            QVector3D d = (o - dl * ((float)i + 0.5f) - du * ((float)j + 0.5f) - position).normalized();
            ans.setPixel(i, j, trace(Ray(position, d)).rgb());
        }

    return ans;
}