#include "GPU/Mat.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string Mat::loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int Mat::compileShaders(const std::string& vsPath, const std::string& fsPath) {
    std::string vsrc = loadFile(vsPath);
    std::string fsrc = loadFile(fsPath);

    const char* v = vsrc.c_str();
    const char* f = fsrc.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v, nullptr);
    glCompileShader(vertex);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f, nullptr);
    glCompileShader(fragment);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}
