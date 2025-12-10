#include "GPU/Tex.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Tex::Tex() {}

Tex::~Tex() {
    if (texID != 0)
        glDeleteTextures(1, &texID);
}

bool Tex::load(const std::string& path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Error cargando textura: " << path << "\n";
        return false;
    }

    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 1) format = GL_RED;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Parámetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar textura
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::cout << glGetError() << std::endl;
    stbi_image_free(data);
    return true;

}

void Tex::use(unsigned int unit) const {
    if (texID == 0) {
        std::cerr << "Advertencia: textura no cargada\n";
        return;
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texID);

}