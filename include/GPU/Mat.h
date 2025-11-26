#ifndef MAT_H
#define MAT_H

#include <string>
#include <glad/glad.h>

class Mat {
public:
    std::string loadFile(const std::string& path);
    unsigned int compileShaders(const std::string& vsPath, const std::string& fsPath);
};

#endif
