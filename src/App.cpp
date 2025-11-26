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

    // SHADER
    shader = mat.compileShaders("../shaders/basic.vs", "../shaders/basic.fs");

    // FIGURAS (usando tu clase real)
    circle.createCircle(40);
    triangle.createTriangle();
    square.createSquare();

    // TEXTURAS
    tex1 = tex.loadTexture("../textures/Paisaje.jpg");
    tex2 = tex.loadTexture("../textures/Panda.jpg");
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

        // -----------------------------
        // DIBUJAR CÍRCULO
        // -----------------------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);

        circle.bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.getCount());

        // -----------------------------
        // DIBUJAR TRIÁNGULO
        // -----------------------------
        triangle.bind();
        glDrawArrays(GL_TRIANGLES, 0, triangle.getCount());

        // -----------------------------
        // DIBUJAR CUADRADO
        // -----------------------------
        glBindTexture(GL_TEXTURE_2D, tex2);

        square.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, square.getCount());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::cleanup() {
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
}
void App::run() {
    mainLoop();
}
