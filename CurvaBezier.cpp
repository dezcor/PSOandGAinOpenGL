#include "CurvaBezier.hpp"

CurvaBezier::CurvaBezier()
{
    Inicio = true;
    Objeto = nullptr;
    Delete = true;
    Index = 0;
}

CurvaBezier::CurvaBezier(float *Vector, int num):CurvaBezier()
{
    SetPoints(Vector,num);
}
CurvaBezier::CurvaBezier(std::vector<float> &Vector, int num):CurvaBezier()
{
    SetPoints(Vector,num);
}
CurvaBezier::CurvaBezier(std::vector<glm::vec3> &Vector):CurvaBezier()
{
    SetPoints(Vector);
}

CurvaBezier::~CurvaBezier()
{
    glDeleteBuffers(1, &mPointsDraw);
    glDeleteBuffers(1, &mPointsLines);
    if(Objeto != nullptr && Delete) delete Objeto;
}
void CurvaBezier::dcRender(glm::mat4 const &m)
{
    if(Objeto != nullptr)
    {
        dcRenderObject(m);
    }
    if(Delete) drawPoints(m);
}
void CurvaBezier::drawPoints(glm::mat4 const &m) const
{
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m));
    glPointSize(4);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mPointsDraw);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex3),
                        (GLvoid *)offsetof(Vertex3, Vertex3::color) // offset
    );
    glDrawArrays(GL_LINE_STRIP, 0, mPointsTime.size());
    /* Stop using the vertex buffer object */
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    if(drawLines)
    {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mPointsLines);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex3),
                            (GLvoid *)offsetof(Vertex3, Vertex3::color) // offset
        );
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        /* Stop using the vertex buffer object */
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mPointsLines);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex3),
                            (GLvoid *)offsetof(Vertex3, Vertex3::color) // offset
        );
        glDrawArrays(GL_LINE_STRIP, 0, mPoints.size());
        /* Stop using the vertex buffer object */
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
    glPointSize(1);   
}
void CurvaBezier::dcUpdate()
{
    if(!Delete)
    {
       //if(Teminio){ Teminio = false;};
       GenerarPoint(Index);
       Index++; 
       return;
    }
    GenerarPoint();
    if(!Inicio)
        glDeleteBuffers(1,&mPointsDraw);
    else
        Inicio = false;
    // Create two vertex buffer objects
    glGenBuffers(1, &mPointsDraw);
    // Tell OpenGL to copy our array to the buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, mPointsDraw);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3) * mPointsTime.size(), mPointsTime.data(), GL_STATIC_DRAW);
    std::vector<Vertex3> Points;
    for(auto aux : mPoints)
    {
        Vertex3 AUX={aux,ColorRGB(0,1,0)};
        Points.push_back(AUX);   
    }
    // Create two vertex buffer objects
    glGenBuffers(1, &mPointsLines);
    // Tell OpenGL to copy our array to the buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, mPointsLines);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3) * Points.size(), Points.data(), GL_STATIC_DRAW);
}
void CurvaBezier::GenerarPoint()
{
    glm::vec3 point(0);
    double dt = 1 / 50.0, t;
    if(Inicio)
    mPointsTime.resize(1);
    for (size_t j = 0; j < 50; j++)
    {
        t = dt * j;
        point = glm::vec3(0);
        size_t i;
        for (i = 0; i < mPoints.size(); i++)
        {
            point += mVector[i] * mPoints[i] * powf(1 - t, (mPoints.size() - 1) - i) * powf(t, i);
        }
        Vertex3 NewPoint = {point, ColorRGB(1, 0, 0)};
        if(i)
        mPointsTime[0]=NewPoint;
    }
}

void CurvaBezier::GenerarCuba()
{
    size_t N = mPoints.size();
    mVector.clear();
    for (std::size_t i = 0; i < N; i++)
    {
        mVector.push_back(n_i(N - 1, i));
    }
}

void CurvaBezier::SetPoint(double *vec,int N)
{
    const int D = 3;
    mPoints.push_back(glm::vec3(vec[0],vec[1],vec[2]));
}

void CurvaBezier::GenerarPoint(const int Index)
{
    glm::vec3 point(0);
    double dt = 1 / 100.0, t;
    t = dt * Index;
    mPointsTime.resize(1);
    point = glm::vec3(0);
    size_t i;
    for (i= 0; i < mPoints.size(); i++)
    {
        point += mVector[i] * mPoints[i] * powf(1 - t, (mPoints.size() - 1) - i) * powf(t, i);
    }
    Vertex3 NewPoint = {point, ColorRGB(1, 0, 0)};
    if(i)
    mPointsTime[0] = NewPoint;
}

void CurvaBezier::SetPoints(float *Vector, int num)
{
    const int D = 3;
    mPoints.resize( num );
    for (size_t i = 0; i < num; i++)
    {
        mPoints[i].x = Vector[i*D];
        mPoints[i].y = Vector[i*D+1];
        mPoints[i].z = Vector[i*D+2];
    }
    GenerarPoint();
}

void CurvaBezier::SetPoints(std::vector<float> &Vector, int num)
{
    const int D = 3;
    mPoints.resize( num );
    for (size_t i = 0; i < num; i++)
    {
        mPoints[i].x = Vector[i*D];
        mPoints[i].y = Vector[i*D+1];
        mPoints[i].z = Vector[i*D+2];
    }
    GenerarCuba();
}
void CurvaBezier::SetPoints(std::vector<glm::vec3> &Vector)
{
    mPoints = Vector;
    size_t N = mPoints.size();
    mVector.clear();
    GenerarCuba();
}

glm::vec3 CurvaBezier::powPSO(glm::vec3 &Point, uint32_t N)
{
    glm::vec3 PowPoint = glm::vec3(1);
    for (uint32_t i = 0; i < N; i++)
    {
        PowPoint *= Point;
    }

    return PowPoint;
}

int CurvaBezier::factorial(int N)
{
    if (N <= 0)
        return 1;
    return factorial(N - 1) * N;
}

int CurvaBezier::n_i(int N, int i)
{
    return factorial(N) / (factorial(i) * factorial((N)-i));
}

void CurvaBezier::SetObject(Render * object)
{
    Objeto = object;
    Objeto->dcUpdate();
}

void CurvaBezier::dcRenderObject(glm::mat4 const &m)
{
    glm::mat4 scalar = glm::scale(glm::mat4(1.0f),glm::vec3(this->scalar));
    glm::mat4 tras,mvp;
    const int i = Delete?Index:0;
    tras = glm::translate(glm::mat4(1.0f),mPointsTime[i].pos);
    mvp = m*tras*scalar*glm::rotate(glm::mat4(1.0f),glm::radians(float(i*3.6)),glm::vec3(0,1,0));
    Objeto->dcRender(mvp);
}