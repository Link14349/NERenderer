//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_CAMERA_H
#define NERENDERER_CAMERA_H

#include <Manifold.h>
#include <Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

class NERenderer;

class Camera {
public:
    Camera(NERenderer* r, const glm::vec3& a, const glm::vec3& b, float x_ = 0, float y_ = 0, float z_ = 0);
    explicit Camera(NERenderer* r, float x_ = 0, float y_ = 0, float z_ = 0);
    void move(float dx, float dy, float dz);// dx, dy, dz小一点
    void render();
private:
    NERenderer* renderer;
    Manifold* manifold;
    Window* window;
    float x, y, z;
    glm::vec3 A, B;// 投影平面矢量(需要正交化)
    cl_mem clvx;
    cl_mem clvy;
    cl_mem clvz;
    cl_mem clTx;
    cl_mem clTy;
    cl_mem clTz;
    cl_mem clDistance;
};


#endif //NERENDERER_CAMERA_H
