//
// Created by Link on 2022/1/23.
//

#ifndef NERENDER_VERTEXMANAGER_H
#define NERENDER_VERTEXMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class VertexManager {
public:
    // 在完成vao设置前请勿生成其他VertexManager
    VertexManager(unsigned int vlen, float* vertexes, unsigned int ilen, unsigned int* indexes, unsigned int type = GL_STATIC_DRAW);
    void addAttrib(unsigned int attribLen, unsigned int type);
    void enableVertexAttribArray();
    void use() const { glBindVertexArray(VAO); }
    void draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexLen, GL_UNSIGNED_INT, 0);
    }
    void update(unsigned int offset, unsigned int len, void* data) const;
    ~VertexManager() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
private:
    unsigned int VAO{}, VBO{}, EBO{}, vertexLen, indexLen;
    unsigned int stride = 0;
    std::vector<unsigned int> attrLens;
    std::vector<unsigned int> types;
    std::vector<unsigned int> offsets;
};


#endif //NERENDER_VERTEXMANAGER_H
