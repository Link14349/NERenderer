//
// Created by Link on 2022/1/24.
//

#include "Camera.h"
#include <NERenderer.h>
#include <Manifold.h>

Camera::Camera(NERenderer *r, const glm::vec3 &a, const glm::vec3 &b, float x_, float y_, float z_)
    : renderer(r), A(a), B(b), x(x_), y(y_), z(z_), manifold(r->getManifold()), window(r->getWindow()) {
    // 正交化A, B
    B -= A * manifold->vec_product(A, B, x, y, z);
}

Camera::Camera(NERenderer *r, float x_, float y_, float z_)
    : renderer(r), x(x_), y(y_), z(z_), manifold(r->getManifold()), A(1, 0, 0), B(0, 1, 0), window(r->getWindow()) {
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
    for (auto& object : renderer->objects) {
        for (auto& face : object->face) {
        }
    }
}
