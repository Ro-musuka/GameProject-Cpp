#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

#include <GL/glew.h>
#include <SDL3/SDL.h>

Texture::Texture()
    : mTextureID(0), mWidth(0), mHeight(0)
{
}

bool Texture::Load(const std::string &fileName)
{
    int channels = 0;

    unsigned char *image = stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 0);

    if (image == nullptr)
    {
        SDL_Log("stbi_load failed to load image %s", fileName.c_str());
        return false;
    }

    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
                 GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    // Enable linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::Unload()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
