//
// Created by Link on 2022/1/25.
//

#include "Geodesic.h"

void Geodesic::next(float h) {
    float Gamma[3][3][3];
    manifold->Christoffel(Gamma, x, y, z);
    // Runge-Kutta法
    glm::vec3 k1(0, 0, 0);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                k1[i] -= Gamma[i][j][k] * T[j] * T[k];
    manifold->Christoffel(Gamma, x + 0.5f * h * T.x, y + 0.5f * h * T.y, z + 0.5f * h * T.z);
    glm::vec3 k2(0, 0, 0);
    glm::vec3 T1(T + 0.5f * h * k1);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                k2[i] -= Gamma[i][j][k] * T1[j] * T1[k];
    manifold->Christoffel(Gamma, x + 0.5f * h * T1.x, y + 0.5f * h * T1.y, z + 0.5f * h * T1.z);
    glm::vec3 k3(0, 0, 0);
    glm::vec3 T2(T + 0.5f * h * k2);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                k3[i] -= Gamma[i][j][k] * T2[j] * T2[k];
    manifold->Christoffel(Gamma, x + h * T2.x, y + h * T2.y, z + h * T2.z);
    glm::vec3 k4(0, 0, 0);
    glm::vec3 T3(T + h * k3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                k4[i] -= Gamma[i][j][k] * T3[j] * T3[k];
    float dx = 0.16666666666666666f * (T.x + 2.0f * T1.x + 2.0f * T2.x + T3.x) * h;
    float dy = 0.16666666666666666f * (T.y + 2.0f * T1.y + 2.0f * T2.y + T3.y) * h;
    float dz = 0.16666666666666666f * (T.z + 2.0f * T1.z + 2.0f * T2.z + T3.z) * h;
    s += sqrtf(manifold->vec_product(glm::vec3(dx, dy, dz), glm::vec3(dx, dy, dz), x, y, z));
    x += 0.16666666666666666f * (T.x + 2.0f * T1.x + 2.0f * T2.x + T3.x) * h;
    y += 0.16666666666666666f * (T.y + 2.0f * T1.y + 2.0f * T2.y + T3.y) * h;
    z += 0.16666666666666666f * (T.z + 2.0f * T1.z + 2.0f * T2.z + T3.z) * h;
    T += 0.16666666666666666f * (k1 + 2.0f * k2 + 2.0f * k3 + k4) * h;// 0.16666666666666666=1/6
//    manifold->range(x, y, z);
}

void Geodesic::getXYZ(float *p) {
    p[0] = x, p[1] = y, p[2] = z;
    manifold->range(p[0], p[1], p[2]);
}
