//
// Created by Link on 2022/1/24.
//

#include "HypersphereManifold.h"
#include <cmath>

// g = [[r, 0, 0], [0, r * powf(cos(x), 2), 0], [0, 0, r * cos(x) ** 2 * cos(y) ** 2]]
void HypersphereManifold::Metric(glm::mat3x3& g, float x, float y, float z) {
    g[0][0] = r * r;
    g[1][1] = r * r * powf(cos(x), 2);
    g[2][2] = r * r * powf(cos(x), 2) * powf(cos(y), 2);
    g[0][1] = g[0][2] = g[1][0] = g[1][2] = g[2][0] = g[2][1] = 0;
}

void HypersphereManifold::Christoffel(float (*Gamma)[3][3], float x, float y, float z) {
    memset((void*)Gamma, 0, 27 * sizeof(float));
    Gamma[0][1][1]= sin(x)*cos(x);
    Gamma[0][2][2]= sin(x)*cos(x)*powf(cos(y), 2);
    Gamma[1][0][1]= -sin(x)/cos(x);
    Gamma[1][1][0]= -sin(x)/cos(x);
    Gamma[1][2][2]= sin(y)*cos(y);
    Gamma[2][0][2]= -sin(x)/cos(x);
    Gamma[2][1][2]= -sin(y)/cos(y);
    Gamma[2][2][0]= -sin(x)/cos(x);
    Gamma[2][2][1]= -sin(y)/cos(y);
}

void HypersphereManifold::Rotate(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &V, float x, float y, float z, float theta,
                                 glm::vec3 &V_) {
    V_.x = A.x*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta) - (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta)) - A.x*(A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + B.x*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta) + (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta)) - B.x*(B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + V.x;
    V_.y = A.y*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta) - (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta)) - A.y*(A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + B.y*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta) + (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta)) - B.y*(B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + V.y;
    V_.z = A.z*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta) - (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta)) - A.z*(A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + B.z*((A.x*V.x*powf(r, 2) + A.y*V.y*powf(r, 2)*powf(cos(x), 2) + A.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*sin(theta) + (B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2))*cos(theta)) - B.z*(B.x*V.x*powf(r, 2) + B.y*V.y*powf(r, 2)*powf(cos(x), 2) + B.z*V.z*powf(r, 2)*powf(cos(x), 2)*powf(cos(y), 2)) + V.z;
}

void HypersphereManifold::Jacobi(glm::mat3x3 &jacobi, float x, float y, float z, float dx, float dy, float dz) {
    float Gamma[3][3][3];
    Christoffel(Gamma, x, y, z);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            jacobi[i][j] = (i == j) - Gamma[i][j][0] * dx - Gamma[i][j][1] * dy - Gamma[i][j][2] * dz;
}

void HypersphereManifold::range(float &x, float &y, float &z) {
    bool add = false;
    x -= (int)(x * 0.5f * M_1_PI) * 2 * M_PI;
    y -= (int)(y * 0.5f * M_1_PI) * 2 * M_PI;
    while (x > M_PI_2 || x < -M_PI_2) {
        if (x > M_PI_2) {
            x = M_PI - x;
            if (y > M_PI_2) y -= M_PI;
            else y += M_PI;
//        y *= -1;
//        z += M_PI;
//        add = true;
        } else if (x < -M_PI_2) {
            x = -M_PI - x;
            if (y > M_PI_2) y -= M_PI;
            else y += M_PI;
//        y *= -1;
//        z += M_PI;
//        add = true;
        }
    }
    while (y > M_PI_2 || y < -M_PI_2) {
        if (y > M_PI_2) {
            y = M_PI - y;
            if (z > M_PI) z -= M_PI;
            else z += M_PI;
        }
        if (y < -M_PI_2) {
            y = -M_PI - y;
            if (z > M_PI) z -= M_PI;
            else z += M_PI;
        }
    }
    z -= (int)(z * 0.5f * M_1_PI) * 2 * M_PI;
    if (z < -M_PI) z += 2 * M_PI;
    if (z > M_PI) z -= 2 * M_PI;
}


