#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GPU/Geo.h"
#include "GPU/Mat.h"
#include "GPU/Tex.h"

class App {
public:
    App();
    ~App();
    
    void run();
    void init();
    void mainLoop();
    void cleanup();

private:
    GLFWwindow* window;

    unsigned int shader;
    unsigned int tex1, tex2;

    Geo circle;
    Geo triangle;
    Geo square;

    Mat mat;
    Tex tex;
};

#endif
