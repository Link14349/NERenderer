//
// Created by Link on 2022/1/30.
//

#ifndef NERENDERER_MESH_H
#define NERENDERER_MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <VertexManager.h>
#include <Shader.h>

struct Face {
    Face(int x, int y, int z) : a(x), b(y), c(z) { }
    unsigned int a, b, c;// vertex id
};
struct Vertex {
    Vertex(const glm::vec3& p, const glm::vec2& t) : position(p), texCoords(t) { }
    glm::vec3 position;// 指数映射
//    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    enum REF_MOD {
        DIFFUSE, SPECULAR
    };
    struct Texture {
        unsigned int id;
        REF_MOD type;
        aiString path;
    };
    std::vector<Vertex*> vertexes;
    std::vector<unsigned int> indexes;
    std::vector<Texture*> textures;
    explicit Mesh(const std::vector<Vertex*>& vertexes, const std::vector<unsigned int>& indexes, const std::vector<Texture*>& textures);
    void draw(const Shader& shader);
    ~Mesh() {
        for (auto& iter : vertexes) delete iter;
        for (auto& iter : textures) delete iter;
    }
private:
    unsigned int VAO, VBO, EBO;
//    VertexManager vertexManager;
};


#endif //NERENDERER_MESH_H
