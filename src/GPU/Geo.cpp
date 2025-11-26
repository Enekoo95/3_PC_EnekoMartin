#include "GPU/Geo.h"
#include <cmath>

void Geo::createCircle(int segments) {
    count = segments + 2;
    std::vector<float> data;
    data.push_back(0.0f); data.push_back(0.0f); data.push_back(0.0f); data.push_back(0.5f); data.push_back(0.5f);

    for (int i = 0; i <= segments; i++) {
        float ang = (float)i / segments * 6.28318f;
        float x = cos(ang) * 0.3f;
        float y = sin(ang) * 0.3f;
        data.push_back(x);
        data.push_back(y);
        data.push_back(0.0f);
        data.push_back(x * 0.5f + 0.5f);
        data.push_back(y * 0.5f + 0.5f);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Geo::createTriangle() {
    float tri[] = {
        0.0f, 0.4f, 0, 0.5f,1.0f, 1.0f,0.0f,0.0f,
       -0.4f,-0.2f,0, 0.0f,0.0f, 0.0f,1.0f,0.0f,
        0.4f,-0.2f,0, 1.0f,0.0f, 0.0f,0.0f,1.0f
    };
    count = 3;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); glEnableVertexAttribArray(2);
}

void Geo::createSquare() {
    float square[] = {
       -0.3f,-0.3f,0,0,0,
       -0.3f, 0.3f,0,0,1,
        0.3f,-0.3f,0,1,0,
        0.3f, 0.3f,0,1,1
    };
    count = 4;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
}

void Geo::bind() {
    glBindVertexArray(vao);
}
