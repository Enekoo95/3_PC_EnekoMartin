#include "GPU/Terrain.h"
#include <stb_image.h>
#include <iostream>

Terrain::Terrain() {
    VAO = VBO = 0;
    width = height = 0;
    vertexCount = 0;
    texture = 0;
}

bool Terrain::loadHeightmap(const std::string& path) {
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 1);

    if (!data) {
        std::cout << "Error: could not load heightmap, creating flat plane\n";
        width = height = 200;
        heights.resize(width * height, 0.0f); // plano plano
        return false;
    }

    heights.resize(width * height);

    for (int i = 0; i < width * height; i++)
        heights[i] = 0.0f; // plano

    stbi_image_free(data);
    return true;
}

void Terrain::generateMesh(float scale) {
    std::vector<float> vertices;

    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            float y1 = heights[z * width + x] * scale;
            float y2 = heights[z * width + (x + 1)] * scale;
            float y3 = heights[(z + 1) * width + x] * scale;
            float y4 = heights[(z + 1) * width + (x + 1)] * scale;

            float xf = float(x);
            float zf = float(z);

            // Triángulo 1
            vertices.insert(vertices.end(), {
                xf, y1, zf,        xf / width, zf / height,
                xf + 1, y2, zf,      (xf + 1) / width, zf / height,
                xf, y3, zf + 1,      xf / width, (zf + 1) / height
                });

            // Triángulo 2
            vertices.insert(vertices.end(), {
                xf + 1, y2, zf,      (xf + 1) / width, zf / height,
                xf + 1, y4, zf + 1,    (xf + 1) / width, (zf + 1) / height,
                xf, y3, zf + 1,      xf / width, (zf + 1) / height
                });
        }
    }

    vertexCount = vertices.size() / 5; // 3 pos + 2 UV

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Terrain::bind() {
    glBindVertexArray(VAO);
}
