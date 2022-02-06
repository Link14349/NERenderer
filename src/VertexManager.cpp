//
// Created by Link on 2022/1/23.
//

#include "VertexManager.h"

VertexManager::VertexManager(unsigned int vlen, float *vertexes, unsigned int ilen, unsigned int *indexes,
                             unsigned int type) : vertexLen(vlen), indexLen(ilen) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vlen * sizeof(float), vertexes, type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ilen * sizeof(unsigned int), indexes, type);
}

void VertexManager::addAttrib(unsigned int attribLen, unsigned int type) {
    unsigned size;
    switch(type) {
        case GL_FLOAT: size = sizeof(float); break;
        case GL_INT: size = sizeof(int); break;
        case GL_DOUBLE: size = sizeof(double); break;
        case GL_UNSIGNED_INT: size = sizeof(unsigned int); break;
        default:
            std::cerr << "ERROR::VERTEX_MANAGER::ADD_ATTRIB::UNKNOWN_DATA_TYPE" << std::endl;
            return;
    }
    attrLens.push_back(attribLen);
    types.push_back(type);
    offsets.push_back(stride);
    stride += size * attribLen;
}

void VertexManager::enableVertexAttribArray() {
    for (unsigned i = 0; i < attrLens.size(); i++) {
        glVertexAttribPointer(i, attrLens[i], types[i], GL_FALSE, stride, (void*)offsets[i]);
        glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexManager::update(unsigned int offset, unsigned int len, void *data) const {
//    use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset * stride, len * stride, data);
}
