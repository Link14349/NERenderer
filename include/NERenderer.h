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
    explicit NERenderer(Manifold* m, const char* title, int w = 800, int h = 1200) : manifold(m), window(title, w, h), shader("./gl/vertexShader.vs.glsl", "./gl/fragmentShader.fs.glsl") { }
    Manifold* getManifold() { return manifold; }
    Window* getWindow() { return &window; }
    void useCamera(Camera* c) { camera = c; }
    void render();
    void addObject(Object* object) { objects.push_back(object); }
    Shader& getShader() { return shader; }
    ~NERenderer() { }
private:
    Manifold* manifold;
    Window window;
    Shader shader;
    Camera* camera{};
    std::vector<Object*> objects;
};


#endif //NERENDERER_NERENDERER_H
