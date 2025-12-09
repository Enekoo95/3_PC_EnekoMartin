#include "App.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- Constructor / Destructor ---
App::App() {
    init();
}

App::~App() {
    cleanup();
}

// --- Inicialización ---
void App::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Terrain 3D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, 800, 800);
    glEnable(GL_DEPTH_TEST);

    // --- Shaders ---
    if (!mat.compileShaders("../shaders/basic.vs", "../shaders/basic.fs")) {
        std::cerr << "Error al compilar shaders\n";
        exit(-1);
    }

    // --- Terreno ---
    if (!terrain.loadHeightmap("../textures/heightmap.png"))
        std::cout << "Using flat terrain\n";

    terrain.generateMesh(1.0f);

    // --- Texturas ---
    tex1 = tex.loadTexture("../textures/grass.jpg");
    tex2 = tex.loadTexture("../textures/rock.jpg");

    updateCameraVectors();
}

// --- Input ---
void App::processInput(float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float velocity = movementSpeed * dt;
    glm::vec3 forward = glm::normalize(glm::vec3(camFront.x, 0, camFront.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += forward * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= forward * velocity;

    glm::vec3 right = glm::normalize(glm::cross(camFront, camUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += right * velocity;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camPos.y += velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camPos.y -= velocity;
}

// --- Main Loop ---
void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat.use();

        glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 800.0f, 0.1f, 500.0f);
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mat.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(glGetUniformLocation(mat.getID(), "lightDir"), -1.0f, -1.0f, -1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glUniform1i(glGetUniformLocation(mat.getID(), "ourTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glUniform1i(glGetUniformLocation(mat.getID(), "secondTexture"), 1);

        glUniform1i(glGetUniformLocation(mat.getID(), "useSecondTexture"), 1);
        glUniform1i(glGetUniformLocation(mat.getID(), "useTexture"), 1);

        terrain.bind();
        glDrawArrays(GL_TRIANGLES, 0, terrain.getCount());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// --- Limpieza ---
void App::cleanup() {
    if (mat.getID()) glDeleteProgram(mat.getID());
    glfwDestroyWindow(window);
    glfwTerminate();
}

// --- Run ---
void App::run() { mainLoop(); }

// --- Callbacks ---
void App::framebuffer_size_cb(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    xoffset *= app->mouseSensitivity;
    yoffset *= app->mouseSensitivity;

    app->yaw += xoffset;
    app->pitch += yoffset;

    if (app->pitch > 89.0f) app->pitch = 89.0f;
    if (app->pitch < -89.0f) app->pitch = -89.0f;

    app->updateCameraVectors();
}

void App::scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    App* app = GET_APP(window);
    if (!app) return;

    app->fov -= (float)yoffset;
    if (app->fov < 20.0f) app->fov = 20.0f;
    if (app->fov > 90.0f) app->fov = 90.0f;
}

// --- Cámara ---
void App::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);
}
