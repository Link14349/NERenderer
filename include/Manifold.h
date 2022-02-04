//
// Created by Link on 2022/1/24.
//

#ifndef NERENDER_MANIFOLD_H
#define NERENDER_MANIFOLD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenCL.h>

class Manifold {
public:
    Manifold() = default;
    virtual void Metric(glm::mat3x3& g, float x, float y, float z) = 0;
    virtual void Christoffel(float Gamma[3][3][3], float x, float y, float z) = 0;// Gamma[mu][alpha][beta]: \Gamma^{\mu}_{\alpha\beta}
    virtual void Rotate(const glm::vec3& A, const glm::vec3& B, const glm::vec3& V, float x, float y, float z, float theta, glm::vec3& V_) = 0;// A, B定义旋转平面(A,B要求正交), V为输入向量, V_为旋转后向量
    virtual void Jacobi(glm::mat3x3& jacobi, float x, float y, float z, float dx, float dy, float dz) = 0;// 向量平移的雅可比矩阵
    virtual float vec_product(const glm::vec3& a, const glm::vec3& b, float x, float y, float z);
    virtual void range(float& x, float& y, float& z) = 0;
//    virtual void Riemann(float R[3][3][3][3]) = 0;
    Program findGeodesic;
    Program getPoint;// 指数映射
protected:
    OpenCL opencl;
};

#endif //NERENDER_MANIFOLD_H
