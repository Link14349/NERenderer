//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_OBJECT_H
#define NERENDERER_OBJECT_H

#include <vector>
#include <Manifold.h>
#include <Shader.h>
#include <VertexManager.h>
#include <Mesh.h>
class NERenderer;

class Object {
    friend class Camera;
public:
    Object(NERenderer* r, float x_, float y_, float z_);
    void move(float dx, float dy, float dz);
    ~Object();
protected:
//    void createVertexManager();
    // vertexes与faces都必须从堆分配内存
    std::vector<Vertex> vertex;
    std::vector<Face> face;
    Manifold* manifold;
//    float* glVertex{};
//    VertexManager* vertexManager;
    float x{}, y{}, z{};
};


#endif //NERENDERER_OBJECT_H
