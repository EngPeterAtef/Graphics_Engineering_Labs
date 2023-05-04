#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.hpp"

GLuint loadImage(const std::string& image_path){

    int w, h, c;
    stbi_uc* pixels = stbi_load(image_path.c_str(), &w, &h, &c, 3);

    if(!pixels) return 0;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, (GLsizei)w, (GLsizei)h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return texture;
}


GLuint fromColor(Color color){

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)&color);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;

}