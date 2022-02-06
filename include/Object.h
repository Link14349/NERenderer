//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_OBJECT_H
#define NERENDERER_OBJECT_H

#define OBJECT_DEBUG

#include <vector>
#include <Manifold.h>
#include <Shader.h>
#include <Texture.h>
#include <VertexManager.h>
#include <map>
class NERenderer;
class Model;

struct Vertex {
    glm::vec3 position;// 指数映射
    glm::vec2 texCoord;
    glm::vec3 screenPosition;
    Vertex(const glm::vec3& p, const glm::vec2& t, const glm::vec3& s) : position(p), texCoord(t), screenPosition(s) { }
};
//struct Face {
//    Face(unsigned x, unsigned y, unsigned z) : a(x), b(y), c(z) { }
//    unsigned a, b, c;
//};

//enum REF_MODE {
//    DIFFUSE, SPECULAR
//};

class Mesh {
public:
    void init();
    void draw(Shader& shader);
    std::vector<Vertex*> vertices;
    std::vector<unsigned int> indexes;
    std::vector<Texture*> textures;
    VertexManager* vertexManager;
    float* glVertex;
    void updateScreenPosition();
//    unsigned int* glIndexes;
    ~Mesh() {
        for (auto& iter : vertices) delete iter;
//        for (auto& iter : textures) delete iter;
        delete vertexManager;
        delete[] glVertex;
//        delete[] glIndexes;
    }
};

class Object {
    friend class Camera;
public:
    Object(NERenderer* r, float x_, float y_, float z_);
    void move(float dx, float dy, float dz);
    void loadModel(const std::string& filename, float scale = 1);
    void updateVertexPosition();
    size_t vertexCount() { return vertexes.size(); }
    void draw(Shader& shader) {
        for (auto& mesh : meshes) mesh->draw(shader);
    }
#ifdef OBJECT_DEBUG
    void debug();
#endif
    ~Object();
protected:
    void updateScreenPosition();
//    void createVertexManager();
    std::vector<Vertex*> vertexes;
    std::vector<Mesh*> meshes;
    Manifold* manifold;
    Model* model = nullptr;
    // 顶点缓存
    float* savedPositionX;
    float* savedPositionY;
    float* savedPositionZ;
//    bool translated = true;
//    float* glVertex{};
//    VertexManager* vertexManager;
    float x{}, y{}, z{};
};


#endif //NERENDERER_OBJECT_H
