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
    void bind();

    void setTexture(unsigned int texID) { texture = texID; }
    void activateTexture() const { glBindTexture(GL_TEXTURE_2D, texture); }

    unsigned int getCount() const { return vertexCount; }

private:
    std::vector<float> heights;
    int width, height;

    unsigned int VAO, VBO;
    unsigned int vertexCount;
    unsigned int texture; // ID de la textura
};

#endif
