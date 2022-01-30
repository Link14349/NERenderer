//
// Created by Link on 2022/1/25.
//

#ifndef NERENDERER_GEODESIC_H
#define NERENDERER_GEODESIC_H

#include <Manifold.h>

class Geodesic {
public:
    Geodesic(Manifold* m, float x_, float y_, float z_, const glm::vec3& T_) : manifold(m), x(x_), y(y_), z(z_), T(T_), s(0) {
        // 归一化
        T /= sqrtf(manifold->vec_product(T, T, x, y, z));
    }
    void next(float ds);
    void getXYZ(float p[3]);
    float s;
private:
    float x, y, z;
    Manifold* manifold;
    glm::vec3 T;
};


#endif //NERENDERER_GEODESIC_H
