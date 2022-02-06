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
    Camera(NERenderer* r, const glm::vec3& a, const glm::vec3& b, float x_ = 0, float y_ = 0, float z_ = 0, float fov = .66666666666f * M_PI_2);
    Camera(NERenderer* r, float x_ = 0, float y_ = 0, float z_ = 0, float fov = .66666666666f * M_PI_2);
    void move(float dx, float dy, float dz);// dx, dy, dz小一点
    void render();
    ~Camera() {
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
    }
private:
    NERenderer* renderer;
    Manifold* manifold;
    Window* window;
    float x, y, z;
    float zLen;// 与fov相关的参数: 视线切向量垂直于视平面分量的值
    size_t preLen = 0;
    glm::vec3 A, B;// 投影平面矢量(需要正交化)
    cl_mem clvx = nullptr;
    cl_mem clvy = nullptr;
    cl_mem clvz = nullptr;
    cl_mem clTx = nullptr;
    cl_mem clTy = nullptr;
    cl_mem clTz = nullptr;
//    cl_mem clMinDistance = nullptr;
    cl_mem clDistance = nullptr;
};


#endif //NERENDERER_CAMERA_H
