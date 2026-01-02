#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "GPU/Terrain.h"
#include "GPU/Mat.h"
#include "GPU/Tex.h"
#include "GPU/Camera.h"
#include "GPU/Water.h"
#include "GPU/Tree.h"

class App {
public:
    App();
    ~App();
    void run();

private:
    void init();
    void mainLoop();
    void cleanup();
    void processInput(float dt);

    static void framebuffer_size_cb(GLFWwindow*, int, int);
    static void mouse_callback(GLFWwindow*, double, double);
    static void scroll_callback(GLFWwindow*, double, double);

    GLFWwindow* window = nullptr;

    // =====================
    // TERRENO
    // =====================
    Terrain terrain;
    float terrainHeightScale = 300.0f;

    Mat mat;
    Tex grass;
    Tex rock;

    // =====================
    // AGUA
    // =====================
    Water* water = nullptr;
    Mat waterMat;
    Tex waterTex;

    // =====================
    // ÁRBOLES
    // =====================
    std::vector<Tree> trees;

    // =====================
    // CÁMARA
    // =====================
    Camera camera;

    float lastX = 400.0f;
    float lastY = 400.0f;
    bool firstMouse = true;
    float lastFrame = 0.0f;
};

inline App* GET_APP(GLFWwindow* w) {
    return reinterpret_cast<App*>(glfwGetWindowUserPointer(w));
}

#endif
