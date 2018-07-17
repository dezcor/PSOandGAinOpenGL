#include "GLTexture.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &path)
{
    if(load(path))
    {
        std::cout << "Error: No se cargo la textura" << std::endl;
    }
}

void Texture::Bin()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

bool Texture::load(const std::string &path)
{
    int W,H,size;
    u_char *bitmap = stbi_load(path.c_str(),&W,&H,&size,4);
    if(bitmap ==NULL)
        return false;
    glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA,W,H,0,GL_RGBA,GL_UNSIGNED_BYTE,bitmap);
    stbi_image_free(bitmap);
    return true;
}