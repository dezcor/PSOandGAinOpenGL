#ifndef _GLTEXTURE_HPP_
#define _GLTEXTURE_HPP_

#include <GL/glew.h>
#include <string>

class Texture
{
    private:
        GLuint texture_id;
    public:
    Texture(){}
    Texture(const std::string &path);

    void Bin();

    bool load(const std::string &path);
};

#endif //_GLTEXTURE_HPP_