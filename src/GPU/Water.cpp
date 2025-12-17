#include "GPU/Water.h"
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

Water::Water(int w, int d, float h)
    : width(w), depth(d), height(h), VAO(0), VBO(0), EBO(0)
{
    generateMesh();
}

Water::~Water() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Water::generateMesh() {
    std::vector<Vertex> vertices(width * depth);
    std::vector<unsigned int> indices;

    // vertices
    for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
            int i = z * width + x;
            vertices[i].pos = glm::vec3((float)x, height, (float)z);
            vertices[i].uv = glm::vec2((float)x / (width - 1), (float)z / (depth - 1));
        }
    }

    // indices
    for (int z = 0; z < depth - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int TL = z * width + x;
            int TR = TL + 1;
            int BL = (z + 1) * width + x;
            int BR = BL + 1;

            indices.push_back(TL); indices.push_back(BL); indices.push_back(TR);
            indices.push_back(TR); indices.push_back(BL); indices.push_back(BR);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Water::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (width - 1) * (depth - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
