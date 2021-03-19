#define TINYOBJLOADER_IMPLEMENTATION
#include "Scene.h"

Scene::Scene(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory);

    /*tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        std::cout << shapes[s].path << std::endl;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
                // Optional: vertex colors
                // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }*/
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
    aiColor3D ka, kd, ks, le;
    float ns;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == AI_SUCCESS)
        std::cout << kd.r << ' ' << kd.g << ' ' << kd.b << std::endl;
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, ks) == AI_SUCCESS)
        std::cout << ks.r << ' ' << ks.g << ' ' << ks.b << std::endl;
    if (material->Get(AI_MATKEY_SHININESS, ns) == AI_SUCCESS)
        std::cout << ns << std::endl;
    if (material->Get(AI_MATKEY_COLOR_EMISSIVE, le) == AI_SUCCESS)
        std::cout << le.r << ' ' << le.g << ' ' << le.b << std::endl;
}
