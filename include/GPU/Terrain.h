#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <string>
#include <glad/glad.h>

class Terrain {
public:
    Terrain();

    bool loadHeightmap(const std::string& path);
    void generateMesh(float scale);

    void bind() const;
    void activateTexture() const { glBindTexture(GL_TEXTURE_2D, texture); }
    void setTexture(unsigned int id) { texture = id; }

    void draw() const;

    unsigned int getIndexCount() const { return indexCount; }

private:
    int width, height;
    std::vector<float> heightData;

    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    unsigned int texture;
};

#endif
