//
// Created by Link on 2022/1/30.
//

#ifndef NERENDERER_MODEL_H
#define NERENDERER_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <Mesh.h>
#include <Shader.h>

unsigned int TextureFromFile(const char* path, const std::string &directory, bool gamma = false);

class Model
{
public:
    /*  函数   */
    explicit Model(const std::string& path) { loadModel(path); }
    ~Model() {
        for (auto& iter : meshes) delete iter;
    }
private:
    /*  模型数据  */
    std::vector<Mesh*> meshes;
    std::string directory;
    /*  函数   */
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, Mesh::REF_MOD typeName, std::vector<Mesh::Texture*>& textures);
};


#endif //NERENDERER_MODEL_H
