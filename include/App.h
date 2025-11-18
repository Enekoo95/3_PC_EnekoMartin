#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class App {
public:
    App();
    ~App();
    void run();

private:
    GLFWwindow* window;
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int texture;

    void init();
    void cleanup();
    void initShaders();
    void initSquare();
    void mainLoop();
    void loadTexture(const std::string& path);
    std::string loadShaderSource(const std::string& path);
};

#endif
