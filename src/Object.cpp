#include <climits>
//
// Created by Link on 2022/1/24.
//

#include "Object.h"
#include <NERenderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Object::move(float dx, float dy, float dz) {
    glm::mat3x3 jacobi;
    manifold->Jacobi(jacobi, x, y, z, dx, dy, dz);
    for (auto &iter : vertex) iter->position = jacobi * iter->position;
    x += dx, y += dy, z += dz;
}

Object::Object(NERenderer *r, float x_, float y_, float z_) : manifold(r->getManifold()), x(x_), y(y_), z(z_) { }

Object::~Object() {
//    delete[] glVertex;
//    delete vertexManager;
}

void Mesh::init() {
    glVertex = new float[5 * vertices.size()];
    for (int i = 0; i < vertices.size(); i++) {
        glVertex[i * 5] = vertices[i]->position.x;
        glVertex[i * 5 + 1] = vertices[i]->position.y;
        glVertex[i * 5 + 2] = vertices[i]->position.z;
        glVertex[i * 5 + 3] = vertices[i]->texCoord.x;
        glVertex[i * 5 + 4] = vertices[i]->texCoord.y;
    }
    glIndexes = new unsigned int[indexes.size()];
    for (int i = 0; i < indexes.size(); i++) {
        glIndexes[i] = indexes[i];
    }
//    vertexManager = new VertexManager(5 * vertices.size(), (float*)&vertices[0], indexes.size(), &indexes[0], GL_DYNAMIC_DRAW);
    vertexManager = new VertexManager(5 * vertices.size(), glVertex, indexes.size(), glIndexes, GL_DYNAMIC_DRAW);
    vertexManager->addAttrib(3, GL_FLOAT);// 3d坐标
    vertexManager->addAttrib(2, GL_FLOAT);// 纹理坐标
    vertexManager->enableVertexAttribArray();
}

void Mesh::draw(Shader& shader) {
    for (unsigned int i = 0; i < textures.size(); i++) {
        textures[i]->use(GL_TEXTURE0 + i);
        shader.setInt("texture" + std::to_string(i), i);
    }
    vertexManager->draw();
}
