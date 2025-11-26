#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GPU/tex.h"
#include "GPU/mat.h"
#include "GPU/geo.h"


class App {
public:
    App();
    ~App();
    void run();

private:
    GLFWwindow* window;


    unsigned int shader;
    unsigned int tex1, tex2, texRotate;
    unsigned int quadVAO;


    void init();
    void createShader();
    void loadTextures();
    void createGeometry();
    void mainLoop();
    void cleanup();
};

#endif
