#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Water {
public:
    Water(int width, int depth, float height);
    ~Water();

    void draw() const;

private:
    void generateMesh();

    unsigned int VAO, VBO, EBO;
    int width, depth;
    float height;
};
