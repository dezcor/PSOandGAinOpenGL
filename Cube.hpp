#ifndef _CUBE_H_
#define _CUBE_H_

#include "Render.hpp"
#include "GLTexture.hpp"

class Cube : public Render
{
    private:
        GLuint Top[3];
        GLuint Bottom[3];
        GLuint Left[3];
        GLuint Right[3];
        GLuint Front[3];
        GLuint Back[3];

        ColorRGB TopColor;
        ColorRGB BottomColor;
        ColorRGB LeftColor;
        ColorRGB RightColor;
        ColorRGB FrontColor;
        ColorRGB BackColor;
        void FreeArray(GLuint *vpo);
        inline void dcRender(const GLuint * vpo);
    public:
        ColorRGB GetTopColor();
        GLvoid SetTopColor(const ColorRGB& RGB);
        ColorRGB GetBottomColor();
        GLvoid SetBottomColor(const ColorRGB& RGB);
        ColorRGB GetLeftColor();
        GLvoid SetLeftColor(const ColorRGB& RGB);
        ColorRGB GetRightColor();
        GLvoid SetRightColor(const ColorRGB& RGB);
        ColorRGB GetFrontColor();
        GLvoid SetFrontColor(const ColorRGB& RGB);
        ColorRGB GetBackColor();
        GLvoid SetBackColor(const ColorRGB& RGB);
        void dcRender(const glm::mat4 &m);
        void dcUpdate();
        
        Cube();
        Cube(const ColorRGB &m);
        ~Cube();
};



#endif //_CUBE_H_