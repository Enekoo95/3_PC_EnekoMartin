#include "App.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// ---------------- Constructor / Destructor ----------------
App::App() { init(); }
App::~App() { cleanup(); }

// ---------------- Init ----------------
void App::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Terrain 3D", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); exit(-1); }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n"; exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    mat.compileShaders("../shaders/basic.vs", "../shaders/basic.fs");

    if (!terrain.loadHeightmap("../textures/heightmap.png"))
        std::cout << "Using flat terrain\n";

    terrain.generateMesh(300.0f);

    grass.load("../textures/grass.png");
    rock.load("../textures/rock.jpg");

    camera = Camera(
        glm::vec3(50.0f, 30.0f, 50.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -135.0f,
        -30.0f
    );
}

// ---------------- Input ----------------
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

// ---------------- Main Loop ----------------
void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat.use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 800.0f, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        glUniform3f(glGetUniformLocation(mat.getID(), "lightDir"), -10.0f, -1.0f, -1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass.getID());
        glUniform1i(glGetUniformLocation(mat.getID(), "ourTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rock.getID());
        glUniform1i(glGetUniformLocation(mat.getID(), "secondTexture"), 1);

        terrain.bind();
        terrain.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ---------------- Cleanup ----------------
void App::cleanup() {
    if (mat.getID()) glDeleteProgram(mat.getID());
    glfwDestroyWindow(window);
    glfwTerminate();
}

// ---------------- Run ----------------
void App::run() { mainLoop(); }

// ---------------- Callbacks ----------------
void App::framebuffer_size_cb(GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); }

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
