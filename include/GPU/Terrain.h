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

    // Altura mínima REAL del terreno (para el agua)
    float getMinHeight(float scale) const;

    // Tamaño del terreno (para agua y árboles)
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Altura exacta del terreno en una coordenada (árboles)
    float getHeightAt(int x, int z) const;

private:
    int width = 0;
    int height = 0;
    std::vector<float> heightData;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int indexCount = 0;
};

#endif
