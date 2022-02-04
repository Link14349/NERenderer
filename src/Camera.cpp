//
// Created by Link on 2022/1/24.
//

#include "Camera.h"
#include <NERenderer.h>
#include <Manifold.h>

Camera::Camera(NERenderer *r, const glm::vec3 &a, const glm::vec3 &b, float x_, float y_, float z_)
    : renderer(r), A(a), B(b), x(x_), y(y_), z(z_), manifold(r->getManifold()), window(r->getWindow()) {
    // 归一化A, B
    A /= sqrtf(manifold->vec_product(A, A, x, y, z));
    B /= sqrtf(manifold->vec_product(B, B, x, y, z));
    // 正交化A, B
    B -= A * manifold->vec_product(A, B, x, y, z);
}

Camera::Camera(NERenderer *r, float x_, float y_, float z_)
    : renderer(r), x(x_), y(y_), z(z_), manifold(r->getManifold()), A(1, 0, 0), B(0, 1, 0), window(r->getWindow()) {
    // 归一化A, B
    A /= sqrtf(manifold->vec_product(A, A, x, y, z));
    B /= sqrtf(manifold->vec_product(B, B, x, y, z));
    // 正交化A, B
    B -= A * manifold->vec_product(A, B, x, y, z);
}

void Camera::move(float dx, float dy, float dz) {
    glm::mat3x3 jacobi;
    manifold->Jacobi(jacobi, x, y, z, dx, dy, dz);
    A = jacobi * A;
    B = jacobi * B;
    B -= A * manifold->vec_product(A, B, x, y, z);// 正交化(否则计算可能存在数值误差)
    x += dx, y += dy, z += dz;
}

void Camera::render() {
    auto& findGeodesic = manifold->findGeodesic;
    size_t len = 0;
    // 计算过各顶点的测地线
    for (auto& object : renderer->objects) len += object->vertexes.size();
    float vx[len];
    float vy[len];
    float vz[len];
    float Tx[len];
    float Ty[len];
    float Tz[len];
    float distance[len];
    size_t i = 0;
    for (auto& object : renderer->objects) {
        for (size_t j = 0; j < object->vertexCount(); j++, i++) {
            vx[i] = object->savedPositionX[j];
            vy[i] = object->savedPositionY[j];
            vz[i] = object->savedPositionZ[j];
            // 第一次猜测测地线于(x, y, z)的切矢
            Tx[i] = object->savedPositionX[j] - x;
            Ty[i] = object->savedPositionY[j] - y;
            Tz[i] = object->savedPositionZ[j] - z;
            float Tlen = 1.0f / sqrtf(manifold->vec_product(glm::vec3(Tx[i], Ty[i], Tz[i]), glm::vec3(Tx[i], Ty[i], Tz[i]), x, y, z));
            Tx[i] *= Tlen;
            Ty[i] *= Tlen;
            Tz[i] *= Tlen;
        }
    }
    // 猜测
    // some code
}
