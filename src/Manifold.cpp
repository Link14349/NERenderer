//
// Created by Link on 2022/1/24.
//

#include "Manifold.h"

float Manifold::vec_product(const glm::vec3& a, const glm::vec3& b, float x, float y, float z) {
    float result = 0;
    glm::mat3x3 g;
    Metric(g, x, y, z);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result += g[i][j] * a[i] * b[j];
    return result;
}
