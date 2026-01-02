#include "GPU/Terrain.h"
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

Terrain::Terrain() {}

bool Terrain::loadHeightmap(const std::string& path) {
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 1);

    if (!data) {
        std::cout << "Failed to load heightmap. Using flat terrain.\n";
        width = height = 128;
        heightData.assign(width * height, 0.0f);
        return false;
    }

    heightData.resize(width * height);
    for (int i = 0; i < width * height; ++i)
        heightData[i] = data[i] / 255.0f;

    stbi_image_free(data);
    return true;
}

void Terrain::generateMesh(float scale) {
    if (width <= 1 || height <= 1) return;

    // ======================
    // Altura mínima
    // ======================
    float minH = heightData[0];
    for (float h : heightData)
        minH = std::min(minH, h);

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    std::vector<Vertex> vertices(width * height);
    std::vector<unsigned int> indices;

    float offsetX = width * 0.5f;
    float offsetZ = height * 0.5f;

    // ======================
    // VÉRTICES
    // ======================
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            int i = z * width + x;

            float h = (heightData[i] - minH) * scale;

            vertices[i].pos = glm::vec3(
                x - offsetX,
                h,
                z - offsetZ
            );

            vertices[i].uv = glm::vec2(
                (float)x / (width - 1),
                (float)z / (height - 1)
            );

            vertices[i].normal = glm::vec3(0.0f);
        }
    }

    // ======================
    // NORMALES
    // ======================
    for (int z = 1; z < height - 1; ++z) {
        for (int x = 1; x < width - 1; ++x) {
            int i = z * width + x;

            float hl = vertices[i - 1].pos.y;
            float hr = vertices[i + 1].pos.y;
            float hd = vertices[i - width].pos.y;
            float hu = vertices[i + width].pos.y;

            glm::vec3 n(hl - hr, 2.0f, hd - hu);
            vertices[i].normal = glm::normalize(n);
        }
    }

    // ======================
    // ÍNDICES
    // ======================
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            unsigned int TL = (unsigned int)(z * width + x);
            unsigned int TR = TL + 1;
            unsigned int BL = (unsigned int)((z + 1) * width + x);
            unsigned int BR = BL + 1;

            indices.push_back(TL);
            indices.push_back(BL);
            indices.push_back(TR);

            indices.push_back(TR);
            indices.push_back(BL);
            indices.push_back(BR);
        }
    }

    indexCount = (unsigned int)indices.size();

    // ======================
    // OPENGL BUFFERS
    // ======================
    if (!VAO) glGenVertexArrays(1, &VAO);
    if (!VBO) glGenBuffers(1, &VBO);
    if (!EBO) glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Terrain::bind() const {
    glBindVertexArray(VAO);
}

void Terrain::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

float Terrain::getMinHeight(float scale) const {
    if (heightData.empty()) return 0.0f;

    float minH = heightData[0];
    for (float h : heightData)
        minH = std::min(minH, h);

    return (minH - minH) * scale; // siempre 0
}

float Terrain::getHeightAt(int x, int z) const {
    if (x < 0 || z < 0 || x >= width || z >= height)
        return 0.0f;

    float minH = heightData[0];
    for (float h : heightData)
        minH = std::min(minH, h);

    return (heightData[z * width + x] - minH) * 300.0f;
}
