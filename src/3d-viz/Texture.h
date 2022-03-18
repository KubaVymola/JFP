#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Texture {
public:
    Texture(const char *texturePath);
    void Load(const char *texturePath);
    GLuint getTextureID() { return _textureID; }
    unsigned char* getData() { return _data; }
private:
    unsigned char *_data;
    GLuint _textureID;
};

#endif