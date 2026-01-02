#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include "GPU/Model.h"

class Tree {
public:
    Tree(const std::string& objPath, glm::vec3 pos, float scale);

    void draw(unsigned int shaderID) const;

private:
    glm::vec3 position;
    float scale;
    Model model;
};

#endif
