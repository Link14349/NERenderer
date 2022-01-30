//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_HYPERSPHEREMANIFOLD_H
#define NERENDERER_HYPERSPHEREMANIFOLD_H

#include <Manifold.h>

class HypersphereManifold : public Manifold {
public:
    explicit HypersphereManifold(float radius = 1) : r(radius) { }
    void Metric(glm::mat3x3& g, float x, float y, float z) override;
    void Christoffel(float Gamma[3][3][3], float x, float y, float z) override;// Gamma[mu][alpha][beta]: \Gamma^{\mu}_{\alpha\beta}
    void Rotate(const glm::vec3& A, const glm::vec3& B, const glm::vec3& V, float x, float y, float z, float theta, glm::vec3& V_) override;// A, B定义旋转平面(A,B要求正交), V为输入向量, V_为旋转后向量
    void Jacobi(glm::mat3x3& jacobi, float x, float y, float z, float dx, float dy, float dz) override;
    void range(float& x, float& y, float& z) override ;
private:
    float r;
};


#endif //NERENDERER_HYPERSPHEREMANIFOLD_H
