#include "GPU/Geo.h"
#include <cmath>

void Geo::createCube() {
    float vertices[] = {
        // posiciones          // UVs
        -0.5f, -0.5f, -0.5f,  0,0,
         0.5f, -0.5f, -0.5f,  1,0,
         0.5f,  0.5f, -0.5f,  1,1,
         0.5f,  0.5f, -0.5f,  1,1,
        -0.5f,  0.5f, -0.5f,  0,1,
        -0.5f, -0.5f, -0.5f,  0,0,

        -0.5f, -0.5f,  0.5f,  0,0,
         0.5f, -0.5f,  0.5f,  1,0,
         0.5f,  0.5f,  0.5f,  1,1,
         0.5f,  0.5f,  0.5f,  1,1,
        -0.5f,  0.5f,  0.5f,  0,1,
        -0.5f, -0.5f,  0.5f,  0,0,

        -0.5f,  0.5f,  0.5f,  1,0,
        -0.5f,  0.5f, -0.5f,  1,1,
        -0.5f, -0.5f, -0.5f,  0,1,
        -0.5f, -0.5f, -0.5f,  0,1,
        -0.5f, -0.5f,  0.5f,  0,0,
        -0.5f,  0.5f,  0.5f,  1,0,

         0.5f,  0.5f,  0.5f,  1,0,
         0.5f,  0.5f, -0.5f,  1,1,
         0.5f, -0.5f, -0.5f,  0,1,
         0.5f, -0.5f, -0.5f,  0,1,
         0.5f, -0.5f,  0.5f,  0,0,
         0.5f,  0.5f,  0.5f,  1,0,

        -0.5f, -0.5f, -0.5f,  0,1,
         0.5f, -0.5f, -0.5f,  1,1,
         0.5f, -0.5f,  0.5f,  1,0,
         0.5f, -0.5f,  0.5f,  1,0,
        -0.5f, -0.5f,  0.5f,  0,0,
        -0.5f, -0.5f, -0.5f,  0,1,

        -0.5f,  0.5f, -0.5f,  0,1,
         0.5f,  0.5f, -0.5f,  1,1,
         0.5f,  0.5f,  0.5f,  1,0,
         0.5f,  0.5f,  0.5f,  1,0,
        -0.5f,  0.5f,  0.5f,  0,0,
        -0.5f,  0.5f, -0.5f,  0,1
    };

    count = 36; // 12 triángulos * 3 vertices

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}



void Geo::bind() const {
    glBindVertexArray(vao);
}
