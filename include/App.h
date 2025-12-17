#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include "GPU/Terrain.h"
#include "GPU/Mat.h"
#include "GPU/Tex.h"
#include "GPU/Camera.h"
#include "GPU/Water.h"


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

    static void framebuffer_size_cb(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* window;

    Terrain terrain;
    Mat mat;
    Tex grass;
    Tex rock;

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
