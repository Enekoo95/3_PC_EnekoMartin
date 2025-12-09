#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GPU/Terrain.h"
#include "GPU/Mat.h"
#include "GPU/Tex.h"

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
    void updateCameraVectors();

    static void framebuffer_size_cb(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* window;

    unsigned int shader;
    unsigned int tex1, tex2;

    Terrain terrain;
    Mat mat;
    Tex tex;

    // Camera
    glm::vec3 camPos = glm::vec3(50.0f, 30.0f, 50.0f);
    glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -135.0f;
    float pitch = -30.0f;
    float fov = 45.0f;
    float movementSpeed = 20.0f;
    float mouseSensitivity = 0.1f;

    float lastX = 400, lastY = 400;
    bool firstMouse = true;
    float lastFrame = 0.0f;
};

// Helper to obtain App* from GLFW window pointer
inline App* GET_APP(GLFWwindow* w) { return reinterpret_cast<App*>(glfwGetWindowUserPointer(w)); }

#endif
