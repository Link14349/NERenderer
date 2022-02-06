//
// Created by Link on 2022/1/24.
//

#include "NERenderer.h"

void NERenderer::render() {
    if (!camera) {
        std::cerr << "ERROR::NERENDERER::RENDER::NO_CAMERA" << std::endl;
        return;
    }
    shader.use();
    camera->render();
}
