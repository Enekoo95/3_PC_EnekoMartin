#include "App.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

App::App() {
    init();
}

App::~App() {
    cleanup();
}

void App::init() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to initialize GLFW\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Figuras", nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR: Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: Failed to initialize GLAD\n";
        exit(-1);
    }

    glViewport(0, 0, 800, 800);

    // Shaders
    shader = Mat::compileShaders("../shaders/basic.vs", "../shaders/basic.fs");

    // Geometría
    Geo::creatCircle(vaoCircle, vboCircle, circleCount);
    Geo::creatTriangle(vaoTriangle, vboTriangle);
    Geo::creatSquare(vaoSquare, vboSquare, vaoRotSquare, vboRotSquare);

    // Texturas
    tex1 = Tex::load("../textures/Paisaje.jpg");
    tex2 = Tex::load("../textures/Panda.jpg");
    texRotate = Tex::load("../textures/Texture2.jpg");
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::ortho(-1.f, 1.f, -1.f, 1.f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(proj));

        // Dibujar figuras usando GEO
        Geo::drawCircle(vaoCircle, circleCount, shader);
        Geo::drawTriangle(vaoTriangle, shader);
        Geo::drawSquare(vaoSquare, shader, tex1, tex2);
        Geo::drawRotatingSquare(vaoRotSquare, shader, texRotate);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::cleanup() {
    glDeleteVertexArrays(1, &vaoCircle);
    glDeleteVertexArrays(1, &vaoTriangle);
    glDeleteVertexArrays(1, &vaoSquare);
    glDeleteVertexArrays(1, &vaoRotSquare);
    glDeleteProgram(shader);

    glfwDestroyWindow(window);
    glfwTerminate();
}