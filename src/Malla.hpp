#ifndef _MALLA_H_
#define _MALLA_H_

#include "Render.hpp"
#include <vector>
#include <glm/glm.hpp>
using namespace std;
class Malla : public Render
{
    private:
        GLuint mMalla[3];
        bool mInicio;
        ColorRGB mColor;
        glm::vec2 mLimitesX;
        glm::vec2 mLimitesY;
        glm::vec2 mNumberTheLines;

        void FreeArray(GLuint *vpo);
        inline void dcRender(const GLuint * vpo);
        double (*funcion)(double * X,int N);
        static double normal(double * X, int N){ return 0;}
        glm::vec3 Scalar;
    public:
        ColorRGB GetColor();
        GLvoid SetColor(const ColorRGB& RGB);
        void dcRender(glm::mat4 const &m);
        void dcUpdate();
        void SetFuncion( double (*f)(double * X,int N));
        void SetLimitesX(float xi,float xf);
        void SetLimitesY(float yi,float yf);
        void SetNumberLines(int N);
        unsigned int GetNumberLines();
        glm::vec3 getScalar(){ return Scalar; }
        Malla();
        Malla(GLuint MatrixID);
        ~Malla();
};



#endif //_Malla_H_