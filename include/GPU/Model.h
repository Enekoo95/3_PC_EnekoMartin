#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Model {
public:
    Model(const std::string& path);
    ~Model();

    void draw() const;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    void loadOBJ(const std::string& path);
};

#endif
