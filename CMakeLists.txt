cmake_minimum_required(VERSION 3.16)
project(NERenderer)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/include/ ./include ./)
link_directories(/usr/local/lib)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_executable(NERenderer main.cpp include/glad/glad.h include/KHR/khrplatform.h src/glad.c include/Shader.h src/Shader.cpp src/Window.cpp include/Window.h src/VertexManager.cpp include/VertexManager.h include/stb_image.h src/Texture.cpp include/Texture.h src/Manifold.cpp include/Manifold.h HypersphereManifold/HypersphereManifold.cpp HypersphereManifold/HypersphereManifold.h src/NERenderer.cpp include/NERenderer.h src/Camera.cpp include/Camera.h src/Object.cpp include/Object.h src/Geodesic.cpp include/Geodesic.h src/Mesh.cpp include/Mesh.h src/Model.cpp include/Model.h)
target_link_libraries(NERenderer glfw)
target_link_libraries(NERenderer assimp)