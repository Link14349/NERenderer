//
// Created by Link on 2022/1/24.
//

#ifndef NERENDERER_NERENDERER_H
#define NERENDERER_NERENDERER_H

#include <Manifold.h>
#include <Camera.h>
#include <Window.h>
#include <Object.h>
#include <vector>

class NERenderer {
public:
    friend class Camera;
    explicit NERenderer(Manifold* m, const char* title) : manifold(m), window(title) { }
    Manifold* getManifold() { return manifold; }
    Window* getWindow() { return &window; }
    void useCamera(Camera* c) { camera = c; }
    void render();
    ~NERenderer() { }
private:
    Manifold* manifold;
    Window window;
    Camera* camera{};
    std::vector<Object*> objects;
};


#endif //NERENDERER_NERENDERER_H
