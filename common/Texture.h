#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
    Texture();
    Texture(const char *fileLoc);
    ~Texture();

    bool LoadTextureA();
    bool LoadTexture();

    void UseTexture();
    void ClearTexture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    const char *fileLocation;
};

#endif
