#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <string>
#include <glad/glad.h>

class Terrain {
public:
    Terrain();

    // Carga el heightmap
    bool loadHeightmap(const std::string& path);

    // Genera la malla del terreno
    void generateMesh(float scale);

    // Render
    void bind() const;
    void draw() const;

    // Altura mínima REAL del terreno
    float getMinHeight(float scale) const;

    // Tamaño del terreno (para ajustar agua)
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width, height;
    std::vector<float> heightData;

    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
};

#endif
