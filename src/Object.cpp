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
    for (auto &iter : vertex) iter.position = jacobi * iter.position;
    x += dx, y += dy, z += dz;
}

Object::Object(NERenderer *r, float x_, float y_, float z_) : manifold(r->getManifold()), x(x_), y(y_), z(z_) { }

Object::~Object() {
//    delete[] glVertex;
//    delete vertexManager;
}
