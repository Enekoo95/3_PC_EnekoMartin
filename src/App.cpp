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

    window = glfwCreateWindow(800, 800, "Cubo 3D Angulado", nullptr, nullptr);
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
    glEnable(GL_DEPTH_TEST); // profundidad para 3D

    // Shaders
    shader = mat.compileShaders("../shaders/basic.vs", "../shaders/basic.fs");

    // Crear cubo
    square.createCube();

    // Cargar textura
    tex1 = tex.loadTexture("../textures/Panda.jpg");
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        // Cámara fija desde un ángulo usando lookAt
        glm::vec3 camPos = glm::vec3(2.0f, 2.0f, 2.0f); // posición de cámara
        glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f); // hacia el centro
        glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f); // arriba

        glm::mat4 view = glm::lookAt(camPos, camTarget, camUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 800.f, 0.1f, 100.f);

        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(glGetUniformLocation(shader, "ourTexture"), 0);

        // Cubo centrado, sin rotación
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Dibujar cubo
        glBindTexture(GL_TEXTURE_2D, tex1);
        square.bind();
        glDrawArrays(GL_TRIANGLES, 0, square.getCount());

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
