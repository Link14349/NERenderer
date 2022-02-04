//
// Created by Link on 2022/1/30.
//

#include <iostream>
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Model::Model(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    auto m = new Mesh;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if(mesh->mTextureCoords[0]) {
            m->vertices.push_back(new Vertex(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y), glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z)));
        } else {
            m->vertices.push_back(new Vertex(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), glm::vec2(0, 0), glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z)));
        }
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            m->indexes.push_back(face.mIndices[j]);
    }
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(material, aiTextureType_DIFFUSE, m->textures);
        loadMaterialTextures(material, aiTextureType_SPECULAR, m->textures);
    }
    m->init();
    return m;
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::vector<Texture *> &textures) {
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        Texture* t;
        bool flag = true;
        mat->GetTexture(type, i, &str);
        for (auto& iter : loadedTexture) {
            if (iter->path == directory + "/" + str.C_Str()) {
                t = iter;
                flag = false;
                break;
            }
        }
        if (flag) {
            t = new Texture(directory + "/" + str.C_Str());
            loadedTexture.push_back(t);
        }
        textures.push_back(t);
        loadedTexture.push_back(t);
//        texture->type = typeName;
//        texture->path = str;
    }
}

void Model::draw(Shader& shader) {
    for (auto& mesh : meshes)
        mesh->draw(shader);
}
