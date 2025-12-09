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

bool Mat::compileShaders(const std::string& vsPath, const std::string& fsPath) {
    std::string vsrc = loadFile(vsPath);
    std::string fsrc = loadFile(fsPath);

    if (vsrc.empty() || fsrc.empty()) return false;

    const char* v = vsrc.c_str();
    const char* f = fsrc.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v, nullptr);
    glCompileShader(vertex);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "Error compilando Vertex Shader:\n" << infoLog << "\n";
        return false;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "Error compilando Fragment Shader:\n" << infoLog << "\n";
        return false;
    }

    MatID = glCreateProgram();
    glAttachShader(MatID, vertex);
    glAttachShader(MatID, fragment);
    glLinkProgram(MatID);

    glGetProgramiv(MatID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(MatID, 512, nullptr, infoLog);
        std::cerr << "Error linking Shader Program:\n" << infoLog << "\n";
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}
