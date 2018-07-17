#include "Cube.hpp"

Cube::Cube()
{
    Top[2]=0;
    Bottom[2]=0;
    Left[2]=0;
    Right[2]=0;
    Front[2]=0;
    Back[2]=0;
    SetTopColor(ColorRGB(0,0,1));
    SetBottomColor(ColorRGB(1,0,1));
    SetLeftColor(ColorRGB(1,1,0));
    SetRightColor(ColorRGB(0,1,0));
    SetFrontColor(ColorRGB(1,0,0));
    SetBackColor(ColorRGB(0,1,1));
    if(!GLtext.load("res_texture.png"))
    std::cout << "Error: No se cargaron las texturas" << std::endl;
}

Cube::Cube(const ColorRGB & color)
{
    Top[2]=0;
    Bottom[2]=0;
    Left[2]=0;
    Right[2]=0;
    Front[2]=0;
    Back[2]=0;
    SetTopColor(color);
    SetBottomColor(color);
    SetLeftColor(color);
    SetRightColor(color);
    SetFrontColor(color);
    SetBackColor(color);
}

 Cube::Cube(const ColorRGB &m,const std::string &path):Cube(m)
 {
     if(!GLtext.load(path))
     std::cout << "Error: No se cargaron las texturas" << std::endl;
 }

Cube::~Cube()
{
    FreeArray(Top);
    FreeArray(Bottom);
    FreeArray(Right);
    FreeArray(Left);
    FreeArray(Front);
    FreeArray(Back);
}

ColorRGB Cube::GetTopColor()
{
    return TopColor;
}

GLvoid Cube::SetTopColor(const ColorRGB& RGB)
{
    if (TopColor == RGB)return;
	TopColor = RGB;
    if(Top[2]>0)
        FreeArray(Top);
    else
        Top[2]++;
    Vertex3d vertex[] =
    {
        { vect3(1,1,-1), TopColor,   vect2(0,1)},
        { vect3(-1,1,-1),TopColor,   vect2(1,1) },
        { vect3(1,1,1),TopColor,     vect2(0,0)},
        { vect3(-1 , 1, 1),TopColor, vect2(1,0) }
    };
    glGenBuffers(1, &Top[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Top[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}


ColorRGB Cube::GetBottomColor()
{
    return BottomColor;
}

GLvoid Cube::SetBottomColor(const ColorRGB& RGB)
{
    if (BottomColor == RGB)return;
	BottomColor = RGB;
    if(Bottom[2]>0)
        FreeArray(Bottom);
    else
        Bottom[2]++;
    Vertex3d vertex[] =
    {
        { vect3(1,-1,-1), BottomColor, vect2(0,1)},
        { vect3(-1,-1,-1),BottomColor, vect2(1,1)},
        { vect3(1,-1,1),BottomColor,   vect2(0,0)},
        { vect3(-1 ,-1, 1),BottomColor,vect2(1,0)  }
    };
    glGenBuffers(1, &Bottom[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Bottom[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}


ColorRGB Cube::GetLeftColor()
{
    return LeftColor;
}

GLvoid Cube::SetLeftColor(const ColorRGB& RGB)
{
    if (LeftColor == RGB)return;
	LeftColor = RGB;
    if(Left[2]>0)
        FreeArray(Left);
    else
        Left[2]++;
    Vertex3d vertex[] =
    {
        { vect3(1,-1,-1), LeftColor,  vect2(0,1) },
        { vect3(1,1,-1),LeftColor ,   vect2(1,1) },
        { vect3(-1,-1,-1),LeftColor,  vect2(0,0) },
        { vect3(-1 , 1, -1),LeftColor,vect2(1,0) }
    };
    glGenBuffers(1, &Left[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Left[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}


ColorRGB Cube::GetRightColor()
{
    return RightColor;
}

GLvoid Cube::SetRightColor(const ColorRGB& RGB)
{
    if (RightColor == RGB)return;
	RightColor = RGB;
    if(Right[2]>0)
        FreeArray(Right);
    else
        Right[2]++;
    Vertex3d vertex[] =
    {
        { vect3(1,-1,1), RightColor,  vect2(0,1) },
        { vect3(1,1,1),RightColor ,   vect2(1,1) },
        { vect3(-1,-1,1),RightColor,  vect2(0,0) },
        { vect3(-1 , 1, 1),RightColor,vect2(1,0) }
    };
    glGenBuffers(1, &Right[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Right[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}

ColorRGB Cube::GetFrontColor()
{
    return FrontColor;
}

GLvoid Cube::SetFrontColor(const ColorRGB& RGB)
{
    if (FrontColor == RGB)return;
	FrontColor = RGB;
    if(Front[2]>0)
        FreeArray(Front);
    Vertex3d vertex[] =
    {
        { vect3(1,-1,-1), FrontColor,vect2(0,1) },
        { vect3(1,1,-1),FrontColor  ,vect2(1,1) },
        { vect3(1,-1,1),FrontColor  ,vect2(0,0) },
        { vect3(1 , 1, 1),FrontColor,vect2(1,0) }
    };
    glGenBuffers(1, &Front[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Front[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}

ColorRGB Cube::GetBackColor()
{
    return BackColor;
}

GLvoid Cube::SetBackColor(const ColorRGB& RGB)
{
    if (BackColor == RGB)return;
	BackColor = RGB;
    if(Back[2]>0)
        FreeArray(Back);
    else
        Back[2]++;
    Vertex3d vertex[] =
    {
        { vect3(-1,-1,-1), BackColor,vect2(0,1) },
        { vect3(-1,1,-1),BackColor  ,vect2(1,1) },
        { vect3(-1,-1,1),BackColor  ,vect2(0,0) },
        { vect3(-1 , 1, 1),BackColor,vect2(1,0) }
    };
    glGenBuffers(1, &Back[0]);
    // Los siguientes comandos le darán características especiales al 'vertexbuffer'
    glBindBuffer(GL_ARRAY_BUFFER, Back[0]);
    // Darle nuestros vértices a  OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // Crear y compilar el programa GLSL desde los shaders
}

void Cube::dcRender(const glm::mat4 &m)
{
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m));
    GLtext.Bin();
    dcRender(Top);
    dcRender(Bottom);
    dcRender(Left);
    dcRender(Right);
    dcRender(Front);
    dcRender(Back);
}

void Cube::dcUpdate()
{

}

void Cube::dcRender(const GLuint *vpo)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vpo[0]);
    glVertexAttribPointer(
        0,   // attribute
        3,                   // number of elements per vertex, here (x,y)
        GL_FLOAT,            // the type of each element
        GL_FALSE,            // take our values as-is
        sizeof(Vertex3d),  // next coord2d appears every 5 floats
        0                    // offset of first element
    );
    glVertexAttribPointer(
        1,      // attribute
        3,                      // number of elements per vertex, here (r,g,b)
        GL_FLOAT,               // the type of each element
        GL_FALSE,               // take our values as-is
        sizeof(struct Vertex3d),  // stride
        //(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element
        (GLvoid*) offsetof(Vertex3d, Vertex3d::color)  // offset
    );

    glVertexAttribPointer(
        2,      // attribute
        2,                      // number of elements per vertex, here (r,g,b)
        GL_FLOAT,               // the type of each element
        GL_TRUE,               // take our values as-is
        sizeof(struct Vertex3d),  // stride
        //(GLvoid*) (2 * sizeof(GLfloat))     // offset of first element
        (GLvoid*) offsetof(Vertex3d, Vertex3d::coorText)  // offset
    );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Cube::FreeArray(GLuint *vpo)
{
    glDeleteBuffers(1,&vpo[0]);
}