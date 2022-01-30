#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <iostream>
#include <cmath>
#include <Window.h>
#include <VertexManager.h>
#include <Texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Manifold.h>
#include <HypersphereManifold/HypersphereManifold.h>
#include <Geodesic.h>

int main() {
    HypersphereManifold manifold;
    Geodesic g(&manifold, 0, 0, 0, glm::vec3(0, 1, 0));
//    Geodesic g(&manifold, 0, 0, 0, glm::vec3(0, 1, 1));
    for (float s = 0; s <= 6.28; s += 0.01f, g.next(0.01f)) {
        struct { float x, y, z; } p{};
        g.getXYZ((float*)(&p));
//        std::cout << g.x << ", " << g.y << ", " << g.z << "; [" << g.s << std::endl;
        std::cout << "[" << p.y << ", " << p.z << "],";
    }
    return 0;
}
