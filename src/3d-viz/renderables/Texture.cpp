#include "Texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *texturePath) {
    Load(texturePath);
}

void Texture::Load(const char *texturePath) {
    glGenTextures(1, &_textureID);

    glBindTexture(GL_TEXTURE_2D, _textureID);

    int width, height, nrChannels;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        std::cout << "Loaded texture " << texturePath << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}
