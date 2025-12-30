#include "App.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

App::App() { init(); }
App::~App() { cleanup(); }

void App::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Terrain + Water", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // =====================
    // SHADERS
    // =====================
    mat.compileShaders("../shaders/basic.vs", "../shaders/basic.fs");
    waterMat.compileShaders("../shaders/water.vs", "../shaders/water.fs");

    // =====================
    // TERRENO
    // =====================
    terrain.loadHeightmap("../textures/heightmap.png");
    terrain.generateMesh(terrainHeightScale);

    grass.load("../textures/grass.png");
    rock.load("../textures/rock.jpg");

    // =====================
    // AGUA (ALTURA REAL DEL TERRENO)
    // =====================
    float margin = 100.0f; // para que sobresalga un poco del terreno
    float waterWidth = (float)terrain.getWidth() + margin;
    float waterDepth = (float)terrain.getHeight() + margin;
    float waterY = 20.0f; // altura del agua a nivel del suelo

    water = new Water(waterWidth, waterDepth, waterY);


    waterTex.load("../textures/water.jpg");

    // =====================
    // CÁMARA
    // =====================
    camera = Camera(
        glm::vec3(50.0f, 40.0f, 50.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -135.0f,
        -30.0f
    );
}

void App::processInput(float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, dt);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, dt);
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float time = (float)glfwGetTime();
        float dt = time - lastFrame;
        lastFrame = time;

        processInput(dt);

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            800.0f / 800.0f,
            0.1f,
            1000.0f
        );

        // =====================
        // TERRENO
        // =====================
        mat.use();
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "model"),
            1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        glUniform3f(glGetUniformLocation(mat.getID(), "lightDir"),
            -1.0f, -1.0f, -1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass.getID());
        glUniform1i(glGetUniformLocation(mat.getID(), "ourTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rock.getID());
        glUniform1i(glGetUniformLocation(mat.getID(), "secondTexture"), 1);

        terrain.draw();

        // =====================
        // AGUA
        // =====================
        waterMat.use();
        glUniformMatrix4fv(glGetUniformLocation(waterMat.getID(), "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(waterMat.getID(), "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(waterMat.getID(), "model"),
            1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        glUniform1f(glGetUniformLocation(waterMat.getID(), "time"), time);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTex.getID());
        glUniform1i(glGetUniformLocation(waterMat.getID(), "waterTexture"), 0);

        water->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::cleanup() {
    delete water;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run() {
    mainLoop();
}

void App::framebuffer_size_cb(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

void App::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    App* app = GET_APP(window);
    if (!app) return;

    if (app->firstMouse) {
        app->lastX = (float)xpos;
        app->lastY = (float)ypos;
        app->firstMouse = false;
    }

    float xoffset = (float)xpos - app->lastX;
    float yoffset = app->lastY - (float)ypos;

    app->lastX = (float)xpos;
    app->lastY = (float)ypos;

    app->camera.ProcessMouseMovement(xoffset, yoffset);
}

void App::scroll_callback(GLFWwindow* window, double, double yoffset) {
    App* app = GET_APP(window);
    if (!app) return;

    app->camera.ProcessMouseScroll((float)yoffset);
}
