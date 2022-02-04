//
// Created by Link on 2022/1/30.
//

#ifndef NERENDERER_MODEL_H
#define NERENDERER_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
//#include <Mesh.h>
#include <Shader.h>
#include <Object.h>

//unsigned int TextureFromFile(const char* path, const std::string &directory, bool gamma = false);

class Model {
    friend class Object;
public:
    /*  函数   */
    explicit Model(const std::string& path);
    void draw(Shader& shader);
    ~Model() {
        for (auto& iter : meshes) delete iter;
//        for (auto& iter : loadedTexture) delete iter;
    }
private:
    /*  模型数据  */
    void processNode(aiNode *node, const aiScene *scene);
    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::vector<Texture*> &textures);
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Mesh*> meshes;
    std::string directory;
    std::vector<Texture*> loadedTexture;
};


#endif //NERENDERER_MODEL_H
