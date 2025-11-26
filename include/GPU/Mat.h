#ifndef MAT_H
#define MAT_H

#include <string>
#include <glad/glad.h>

class Mat {
public:
    Mat() = default;
    ~Mat() = default;

    unsigned int compileShaders(const std::string& vsPath, const std::string& fsPath);
    std::string loadFile(const std::string& path);
};

#endif // MAT_H
