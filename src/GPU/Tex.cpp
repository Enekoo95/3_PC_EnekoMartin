#include "GPU/Tex.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int Tex::loadTexture(const std::string& path) {
    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);

    if (!data) {
        std::cerr << "No se pudo cargar textura: " << path << "\n";
        return 0;
    }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, ch == 4 ? GL_RGBA : GL_RGB, w, h,
        0, ch == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texID;
}
