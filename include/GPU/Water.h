#pragma once
#include <glad/glad.h>

class Water {
public:
    Water(float width, float depth, float y);
    ~Water();

    void draw() const;

private:
    unsigned int VAO, VBO, EBO;
    float width, depth, height;
};
