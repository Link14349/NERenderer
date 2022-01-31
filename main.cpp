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
#include <Model.h>

void processInput(Window& window);

int main() {
    Window window("Model");
    Model model("./resources/zelda/scene.gltf");
//    Model model("./resources/augustus/EmperorAugustus.obj");
    Shader shader("./gl/vertexShader.vs", "./gl/fragmentShader.fs");
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float i = 0;
    while (!window.close()) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearDepth(1.0f);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(i), glm::vec3(0.0, 1.0, 0));
//        trans = glm::scale(trans, glm::vec3(0.005, 0.005, 0.005));
        trans = glm::scale(trans, glm::vec3(0.05, 0.05, 0.05));
        shader.setMatrix4fv("transform", trans);
        model.draw(shader);
        i += 0.5f;
        if (i >= 360) i = 0;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        window.update();
    }
    return 0;
}

void processInput(Window& window) {
    if (window.keyPress(GLFW_KEY_ESCAPE)) window.setClose();
}
