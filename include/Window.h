//
// Created by Link on 2022/1/23.
//

#ifndef NERENDER_WINDOW_H
#define NERENDER_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    explicit Window(const char*, int = 800, int = 600);
    bool close() const { return glfwWindowShouldClose(window); }
    bool keyPress(int key) const { return glfwGetKey(window, key) == GLFW_PRESS; }
    void setClose() { glfwSetWindowShouldClose(window, true); }
    void update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ~Window() { glfwTerminate(); }
//private:
    GLFWwindow* window;
};


#endif //NERENDER_WINDOW_H
