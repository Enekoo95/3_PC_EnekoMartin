#include "GPU/Tree.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Tree::Tree(const std::string& objPath, glm::vec3 pos, float s)
    : position(pos), scale(s), model(objPath) {}

void Tree::draw(unsigned int shaderID) const {
    glm::mat4 modelMat(1.0f);
    modelMat = glm::translate(modelMat, position);
    modelMat = glm::scale(modelMat, glm::vec3(scale));

    glUniformMatrix4fv(
        glGetUniformLocation(shaderID, "model"),
        1, GL_FALSE,
        &modelMat[0][0]
    );

    model.draw();
}
