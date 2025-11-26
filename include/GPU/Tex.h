#ifndef TEX_H
#define TEX_H

#include <glad/glad.h>
#include <string>

class Tex {
public:
    Tex() = default;
    ~Tex() = default;

    unsigned int loadTexture(const std::string& path);
};

#endif // TEX_H
