#ifndef TEX_H
#define TEX_H

#include <glad/glad.h>
#include <string>
#include <iostream>

class Tex {
public:
    Tex();
    ~Tex();

    bool load(const std::string& path);   // carga textura y la guarda internamente
    void use(unsigned int unit = 0) const; // activa textura en la unidad deseada
    unsigned int getID() const { return texID; }

private:
    unsigned int texID = 0;
};

#endif
