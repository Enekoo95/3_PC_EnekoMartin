#ifndef MAT_H
#define MAT_H

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Mat {
public:
    Mat() : MatID(0) {}
    ~Mat() { if (MatID) glDeleteProgram(MatID); }

    bool compileShaders(const std::string& vsPath, const std::string& fsPath);
    void use() const { glUseProgram(MatID); }
    unsigned int getID() const { return MatID; }

private:
    std::string loadFile(const std::string& path);
    unsigned int MatID;
};

#endif
