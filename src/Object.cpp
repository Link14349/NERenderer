#include <climits>
//
// Created by Link on 2022/1/24.
//

#include "Object.h"
#include <NERenderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Model.h>

void Object::move(float dx, float dy, float dz) {
    glm::mat3x3 jacobi;
    manifold->Jacobi(jacobi, x, y, z, dx, dy, dz);
    for (auto &iter : vertexes) iter->position = jacobi * iter->position;
    x += dx, y += dy, z += dz;
    updateVertexPosition();
}

Object::Object(NERenderer *r, float x_, float y_, float z_) : manifold(r->getManifold()), x(x_), y(y_), z(z_) { }

Object::~Object() {
    delete model;
//    delete[] glVertex;
//    delete vertexManager;
}

void Object::loadModel(const std::string& filename, float scale) {
    model = new Model(filename);
    std::map<Vertex*, bool> vertexMap;
    for (auto& mesh : model->meshes) {
        meshes.push_back(mesh);
        for (auto& vertex : mesh->vertices) {
            if (!vertexMap[vertex]) {
                vertexMap[vertex] = true;
                vertexes.push_back(vertex);
                float len = sqrtf(vertex->position.x * vertex->position.x + vertex->position.y * vertex->position.y + vertex->position.z * vertex->position.z);
                float len_ = sqrtf(manifold->vec_product(vertex->position, vertex->position, x, y, z));
                vertex->position *= len / len_ * scale;
            }
        }
    }
    // 初始化顶点缓存并计算其位置
    savedPositionX = new float[vertexes.size()];
    savedPositionY = new float[vertexes.size()];
    savedPositionZ = new float[vertexes.size()];
    updateVertexPosition();
}

void Object::updateVertexPosition() {
    // 切向量
    float Tx[vertexes.size()];
    float Ty[vertexes.size()];
    float Tz[vertexes.size()];
    float h = 0.01f;
    auto count = vertexes.size();
    for (size_t i = 0; i < vertexes.size(); i++) {
        Tx[i] = vertexes[i]->position.x;
        Ty[i] = vertexes[i]->position.y;
        Tz[i] = vertexes[i]->position.z;
    }
    auto& getPoint = manifold->getPoint;
    getPoint.setCount(count);
    auto clTx = getPoint.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clTy = getPoint.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clTz = getPoint.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clOx = getPoint.createBuffer<float>(CL_MEM_WRITE_ONLY);
    auto clOy = getPoint.createBuffer<float>(CL_MEM_WRITE_ONLY);
    auto clOz = getPoint.createBuffer<float>(CL_MEM_WRITE_ONLY);
    getPoint.writeBuffer(clTx, Tx);
    getPoint.writeBuffer(clTy, Ty);
    getPoint.writeBuffer(clTz, Tz);
    getPoint.setArg(&clTx);
    getPoint.setArg(&clTy);
    getPoint.setArg(&clTz);
    getPoint.setArg(&clOx);
    getPoint.setArg(&clOy);
    getPoint.setArg(&clOz);
    getPoint.setArg(&x);
    getPoint.setArg(&y);
    getPoint.setArg(&z);
    getPoint.setArg(&h);
    getPoint.setArg(&count);
    getPoint.execute();
    getPoint.readBuffer(clOx, savedPositionX);
    getPoint.readBuffer(clOy, savedPositionY);
    getPoint.readBuffer(clOz, savedPositionZ);
    clReleaseMemObject(clTx);
    clReleaseMemObject(clTy);
    clReleaseMemObject(clTz);
    clReleaseMemObject(clOx);
    clReleaseMemObject(clOy);
    clReleaseMemObject(clOz);
}

#ifdef OBJECT_DEBUG
void Object::debug() {
    for (auto& vertex : vertexes) std::cout << vertex->position.x << ", " << vertex->position.y << ", " << vertex->position.z << std::endl;
    std::cout << "=====" << std::endl;
    for (int i = 0; i < vertexes.size(); i++) std::cout << savedPositionX[i] << ", " << savedPositionY[i] << ", " << savedPositionZ[i] << std::endl;
}
#endif

void Object::updateScreenPosition() {
    for (auto& mesh : meshes) mesh->updateScreenPosition();
}

void Mesh::updateScreenPosition() {
    for (int i = 0; i < vertices.size(); i++) {
        glVertex[i * 5] = vertices[i]->screenPosition.x;
        glVertex[i * 5 + 1] = vertices[i]->screenPosition.y;
        glVertex[i * 5 + 2] = vertices[i]->screenPosition.z;
        glVertex[i * 5 + 3] = vertices[i]->texCoord.x;
        glVertex[i * 5 + 4] = vertices[i]->texCoord.y;
    }
    vertexManager->update(0, vertices.size(), glVertex);
}

void Mesh::init() {
    glVertex = new float[5 * vertices.size()];
    for (int i = 0; i < vertices.size(); i++) {
        glVertex[i * 5] = vertices[i]->screenPosition.x;
        glVertex[i * 5 + 1] = vertices[i]->screenPosition.y;
        glVertex[i * 5 + 2] = vertices[i]->screenPosition.z;
        glVertex[i * 5 + 3] = vertices[i]->texCoord.x;
        glVertex[i * 5 + 4] = vertices[i]->texCoord.y;
    }
//    vertexManager = new VertexManager(5 * vertices.size(), (float*)&vertices[0], indexes.size(), &indexes[0], GL_DYNAMIC_DRAW);
//    vertexManager = new VertexManager(5 * vertices.size(), (float*)vertices.data(), indexes.size(), indexes.data(), GL_DYNAMIC_DRAW);
    vertexManager = new VertexManager(5 * vertices.size(), glVertex, indexes.size(), indexes.data(), GL_DYNAMIC_DRAW);
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
