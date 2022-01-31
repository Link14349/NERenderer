//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_OBJECT_H
#define NERENDERER_OBJECT_H

#include <vector>
#include <Manifold.h>
#include <Shader.h>
#include <Texture.h>
#include <VertexManager.h>
//#include <Mesh.h>
class NERenderer;
class Model;

struct Vertex {
    glm::vec3 position;// 指数映射，但长度为欧式空间长度
    glm::vec2 texCoord;
    Vertex(const glm::vec3& p, const glm::vec2& t) : position(p), texCoord(t) { }
};
struct Face {
    Face(unsigned x, unsigned y, unsigned z) : a(x), b(y), c(z) { }
    unsigned a, b, c;
};

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
    unsigned int* glIndexes;
    ~Mesh() {
        for (auto& iter : vertices) delete iter;
//        for (auto& iter : textures) delete iter;
        delete vertexManager;
        delete[] glVertex;
        delete[] glIndexes;
    }
};

class Object {
    friend class Camera;
public:
    Object(NERenderer* r, float x_, float y_, float z_);
    void move(float dx, float dy, float dz);
    ~Object();
protected:
//    void createVertexManager();
    // vertexes与faces都必须从堆分配内存
    std::vector<Vertex*> vertex;
    std::vector<Face*> face;
    Manifold* manifold;
//    float* glVertex{};
//    VertexManager* vertexManager;
    float x{}, y{}, z{};
};


#endif //NERENDERER_OBJECT_H
