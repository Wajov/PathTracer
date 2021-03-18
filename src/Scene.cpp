#include "Scene.h"

Scene::Scene(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);
}

void Scene::processNode(aiNode *node, const aiScene *scene, std::string &directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory);
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory);
}

void Scene::processMesh(aiMesh *mesh, const aiScene *scene, std::string &directory) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::cout << material->GetName().C_Str() << std::endl;
    aiColor3D kd, ks, le;
    float ns;
/*    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == AI_SUCCESS)
        std::cout << kd.r << ' ' << kd.g << ' ' << kd.b << std::endl;
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, ks) == AI_SUCCESS)
        std::cout << ks.r << ' ' << ks.g << ' ' << ks.b << std::endl;*/
    if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, le) == AI_SUCCESS)
        std::cout << le.r << ' ' << le.g << ' ' << le.b << std::endl;
/*    if (material->Get(AI_MATKEY_SHININESS, ns) == AI_SUCCESS)
        std::cout << ns << std::endl;*/
}