//
// Created by Link on 2022/1/24.
//

#include "Camera.h"
#include <NERenderer.h>
#include <Manifold.h>

Camera::Camera(NERenderer *r, const glm::vec3 &a, const glm::vec3 &b, float x_, float y_, float z_, float fov)
    : renderer(r), A(a), B(b), x(x_), y(y_), z(z_), manifold(r->getManifold()), window(r->getWindow()), zLen(1.0f / tanf(fov * .5f)) {
    // 归一化A, B
    A /= sqrtf(manifold->vec_product(A, A, x, y, z));
    B /= sqrtf(manifold->vec_product(B, B, x, y, z));
    // 正交化A, B
    B -= A * manifold->vec_product(A, B, x, y, z);
}

Camera::Camera(NERenderer *r, float x_, float y_, float z_, float fov)
    : renderer(r), x(x_), y(y_), z(z_), manifold(r->getManifold()), A(1, 0, 0), B(0, 1, 0), window(r->getWindow()), zLen(1.0f / tanf(fov * .5f)) {
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
    float h = 0.01f;
    // 计算过各顶点的测地线
    for (auto& object : renderer->objects) len += object->vertexes.size();
    if (len == 0) return;
    float vx[len];
    float vy[len];
    float vz[len];
    float Tx[len];
    float Ty[len];
    float Tz[len];
    float distance[len];// 摄像机与顶点的距离
    for (auto& object : renderer->objects)
        for (size_t i = 0; i < object->vertexCount(); i++) {
            vx[i] = object->savedPositionX[i];
            vy[i] = object->savedPositionY[i];
            vz[i] = object->savedPositionZ[i];
        }
    // 猜测
    // some code
    findGeodesic.setCount(len);
    if (findGeodesic.getRCount() != preLen) {
        preLen = findGeodesic.getRCount();
        if (clvx) {
            clReleaseMemObject(clvx);
            clReleaseMemObject(clvy);
            clReleaseMemObject(clvz);
            clReleaseMemObject(clTx);
            clReleaseMemObject(clTy);
            clReleaseMemObject(clTz);
//            clReleaseMemObject(clMinDistance);
            clReleaseMemObject(clDistance);
        }
        clvx = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
        clvy = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
        clvz = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
        clTx = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
        clTy = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
        clTz = findGeodesic.createBuffer<float>(CL_MEM_READ_ONLY);
//        clMinDistance = findGeodesic.createBuffer<float>(CL_MEM_READ_WRITE);
        clDistance = findGeodesic.createBuffer<float>(CL_MEM_READ_WRITE);
    }
    findGeodesic.writeBuffer<float>(clvx, vx);
    findGeodesic.writeBuffer<float>(clvy, vy);
    findGeodesic.writeBuffer<float>(clvz, vz);

    findGeodesic.setArg(&clvx);
    findGeodesic.setArg(&clvy);
    findGeodesic.setArg(&clvz);
    findGeodesic.setArg(&clTx);
    findGeodesic.setArg(&clTy);
    findGeodesic.setArg(&clTz);
    findGeodesic.setArg(&clDistance);
    findGeodesic.setArg(&x);
    findGeodesic.setArg(&y);
    findGeodesic.setArg(&z);
    findGeodesic.setArg(&h);
    findGeodesic.setArg(&len);
    findGeodesic.execute();

//    for (size_t i = 0; i < len; i++) {
//        geodesic(vx, vy, vz, Tx, Ty, Tz, distance, x, y, z, h, i, len);
//    }

//    findGeodesic.readBuffer(clMinDistance, minDistance);
    findGeodesic.readBuffer(clTx, Tx);
    findGeodesic.readBuffer(clTy, Ty);
    findGeodesic.readBuffer(clTz, Tz);
    findGeodesic.readBuffer(clDistance, distance);

    size_t i = 0;
    for (auto& object : renderer->objects) {
        for (size_t j = 0; j < object->vertexCount(); j++) {
            glm::vec3 T(Tx[i], Ty[i], Tz[i]);
            float tmpX = manifold->vec_product(T, A, x, y, z), tmpY = manifold->vec_product(T, B, x, y, z);
            glm::vec3 tmp(T - tmpX * A - tmpY * B);
            float tmpZ = sqrtf(manifold->vec_product(tmp, tmp, x, y, z));
            float scale = zLen / tmpZ;
            tmpX *= scale;
            tmpY *= scale;
            object->vertexes[j]->screenPosition.x = tmpX;
            object->vertexes[j]->screenPosition.y = tmpY;
            object->vertexes[j]->screenPosition.z = distance[i];
//            std::cout << tmpX << ", " << tmpY << ", " << distance[i] << std::endl;
            i++;
        }
        object->updateScreenPosition();
        object->draw(renderer->shader);
    }
}
