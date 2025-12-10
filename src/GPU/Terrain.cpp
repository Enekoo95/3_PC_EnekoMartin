#include "GPU/Terrain.h"
#include <stb_image.h>
#include <iostream>

Terrain::Terrain()
    : width(0), height(0), VAO(0), VBO(0), EBO(0), indexCount(0), texture(0) {
}

bool Terrain::loadHeightmap(const std::string& path)
{
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 1);

    if (!data) {
        std::cout << "Failed to load heightmap. Creating flat 128x128 terrain.\n";
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

void Terrain::generateMesh(float scale)
{
    if (width <= 1 || height <= 1) {
        std::cout << "ERROR: heightmap invalid size (" << width << "x" << height << ")\n";
        return;
    }

    struct Vertex {
        float x, y, z;
        float u, v;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(width * height);

    // Crear vértices
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            int i = z * width + x;
            vertices.push_back({
                (float)x,
                heightData[i] * scale,
                (float)z,
                static_cast<float>(x),
                static_cast<float>(z)
                });
        }
    }

    // Crear índices
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int TL = z * width + x;
            int TR = TL + 1;
            int BL = (z + 1) * width + x;
            int BR = BL + 1;

            indices.push_back(TL);
            indices.push_back(BL);
            indices.push_back(TR);

            indices.push_back(TR);
            indices.push_back(BL);
            indices.push_back(BR);
        }
    }

    indexCount = indices.size();

    std::cout << "Mesh generated: " << vertices.size() << " vertices, " << indexCount << " indices.\n";

    // Crear buffers si no existen
    if (VAO == 0) glGenVertexArrays(1, &VAO);
    if (VBO == 0) glGenBuffers(1, &VBO);
    if (EBO == 0) glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Atributos de vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);         // posición
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))); // uv
    glEnableVertexAttribArray(1);

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
