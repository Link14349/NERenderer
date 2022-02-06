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
#include <sys/time.h>
#include <unistd.h>
#ifdef __APPLE__ //Mac OSX has a different name for the header file
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <OpenCL.h>
#include <NERenderer.h>

//int w = 1200, h = 800;
void processInput(Window& window);
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//    w = width;
//    h = height;
//}

// NERenderer load model
#define LOAD_MODEL
//#define GPU_GEODESIC
//#define CPU_GEODESIC
// 欧几里得空间下的load model
//#define LOAD_MODEL_E

int main() {
    struct timeval time;
    HypersphereManifold manifold;
    unsigned int t1, t2;
#ifdef LOAD_MODEL
    NERenderer renderer(&manifold, "NERenderer", 800, 800);
    Object object(&renderer, 0, 0, .5f);
    Camera camera(&renderer, 0, 0, 0);
    renderer.useCamera(&camera);
    renderer.addObject(&object);
    auto& shader = renderer.getShader();
    gettimeofday(&time, NULL);
    t1 = (time.tv_sec * 1000 + time.tv_usec / 1000);
    object.loadModel("./resources/pikamee_voms/scene.gltf", .01f);
    gettimeofday(&time, NULL);
    t2 = (time.tv_sec * 1000 + time.tv_usec / 1000);
    std::cout << "load model used " << t2 - t1 << "ms" << ", " << object.vertexCount() << " vertexes" << std::endl;
//    object.debug();
//    renderer.render();
    while (!renderer.getWindow()->close()) {
        // input
        // -----
        processInput(*renderer.getWindow());

        // render
        // ------
        glClearDepth(1.0f);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
//        trans = glm::scale(trans, glm::vec3(0.05, 0.05, 0.05));
        shader.setMatrix4fv("transform", trans);
        object.move(0, 0, 0.01f);
//        object.move(0.01f, 0, 0);
        renderer.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        renderer.getWindow()->update();
    }
#endif
#ifdef GPU_GEODESIC
    const size_t count = 1e5;
    OpenCL opencl;
    Program program;
    opencl.loadProgram("./HypersphereManifold/geodesic.cl", program);
    program.loadKernel("geodesic", count);
    size_t rCount = program.realCount();

    auto vx = new float[rCount];
    auto vy = new float[rCount];
    auto vz = new float[rCount];
    auto Tx = new float[rCount];
    auto Ty = new float[rCount];
    auto Tz = new float[rCount];
    float x = 0, y = 0, z = 0;
    auto distance = new float[rCount];
    glm::vec3 T(0, 1, 0.1);
    float len = sqrtf(manifold.vec_product(T, T, 0, 0, 0));
    Tx[0] = T.x;
    Ty[0] = T.y / len;
    Tz[0] = T.z / len;
    float h = 0.01f;
    auto clvx = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clvy = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clvz = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clTx = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clTy = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clTz = program.createBuffer<float>(CL_MEM_READ_ONLY);
    auto clDistance = program.createBuffer<float>(CL_MEM_READ_WRITE);
    program.writeBuffer<float>(clvx, vx);
    program.writeBuffer<float>(clvy, vy);
    program.writeBuffer<float>(clvz, vz);
    program.writeBuffer<float>(clTx, Tx);
    program.writeBuffer<float>(clTy, Ty);
    program.writeBuffer<float>(clTz, Tz);

    program.setArg(&clvx);
    program.setArg(&clvy);
    program.setArg(&clvz);
    program.setArg(&clTx);
    program.setArg(&clTy);
    program.setArg(&clTz);
    program.setArg(&clDistance);
    program.setArg(&x);
    program.setArg(&y);
    program.setArg(&z);
    program.setArg(&h);
    program.setArg(&count);

    gettimeofday(&time, NULL);
    t1 = (time.tv_sec * 1000 + time.tv_usec / 1000);

    program.execute();

    gettimeofday(&time, NULL);
    t2 = (time.tv_sec * 1000 + time.tv_usec / 1000);
    std::cout << "gpu computed " << count << " values in " << t2 - t1 << "ms" << std::endl;

    program.readBuffer(clDistance, distance);
#endif
#ifdef CPU_GEODESIC
//    HypersphereManifold manifold(1);
    Geodesic geodesic(&manifold, 0, 0, 0, glm::vec3(0, 1, 1));
//    struct timeval time;
    gettimeofday(&time, NULL);
    t1 = (time.tv_sec * 1000 + time.tv_usec / 1000);
    for (int i = 0; i < 1000; i++) for (float s = 0; s <= 6.3; s += 0.01f, geodesic.next(0.01f)) ;
    gettimeofday(&time, NULL);
    t2 = (time.tv_sec * 1000 + time.tv_usec / 1000);
    std::cout << "cpu computed 1000 values in " << t2 - t1 << "ms" << std::endl;
#endif
#ifdef LOAD_MODEL_E
    Window window("Model", w, h);
    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
   Model model("./resources/link/scene.gltf");
    // Model model("./resources/augustus/EmperorAugustus.obj");
    Shader shader("./gl/vertexShader.vs.glsl", "./gl/fragmentShader.fs.glsl");
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
        trans = glm::translate(trans, glm::vec3(0, -0.8, 0));
        trans = glm::rotate(trans, glm::radians(i), glm::vec3(0.0, 1.0f, 0));
        trans = glm::scale(trans, glm::vec3(0.01, 0.01, 0.01));
        trans = glm::scale(trans, glm::vec3(((float)h) / ((float)w), 1, 1));
//        trans = glm::scale(trans, glm::vec3(0.05, 0.05, 0.05));
        shader.setMatrix4fv("transform", trans);
        model.draw(shader);
        i += 0.5f;
        if (i >= 360) i = 0;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        window.update();
    }
#endif
    return 0;
}

void processInput(Window& window) {
    if (window.keyPress(GLFW_KEY_ESCAPE)) window.setClose();
}
