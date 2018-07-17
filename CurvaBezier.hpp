#ifndef _CURVABEZIER_HPP_
#define _CURVABEZIER_HPP_

#include "Render.hpp"
#include <glm/glm.hpp>
//#include "Cube.hpp"

struct Vertex3
{
    glm::vec3 pos;
    ColorRGB color;
};

class CurvaBezier : public Render
{
private:
    /* data */
    std::vector<glm::vec3> mPoints;
    std::vector<Vertex3> mPointsTime;
    uint mPointsDraw;
    uint mPointsLines;
    bool Inicio;
    std::vector<float> mVector;
    glm::vec3 powPSO(glm::vec3& Point,uint32_t N);
    int factorial(int N);
    int n_i(int N,int i);
    Render * Objeto;
    GLfloat scalar;
    bool drawLines;
    bool Delete;
    int Index;
    bool Teminio = false;
    inline void drawPoints(glm::mat4 const &m) const;

    void dcRenderObject(glm::mat4 const &m);
public:
    CurvaBezier(/* args */);
    CurvaBezier(float *Vector,int num);
    CurvaBezier(std::vector<float>& Vector,int num);
    CurvaBezier(std::vector<glm::vec3>& Vector);
    ~CurvaBezier();
    void dcRender(glm::mat4 const &m);
    void dcUpdate();
    void GenerarPoint();
    glm::vec3 GetPoint(const int index) const { if(mPointsTime.size()>0)return mPointsTime[index].pos;return glm::vec3(0);}
    void GenerarPoint(const int Index);
    void GenerarCuba();
    void SetPoint(double *vec,int N);
    void SetPoints(float *Vector,int num);
    void SetPoints(std::vector<float>& Vector,int num);
    void SetPoints(std::vector<glm::vec3>& Vector);
    void SetObject(Render * object);
    void SetDelete(bool is) {Delete = is;}
    void SetDrawLines(bool is){ drawLines = is; }
    void SetTermino(bool is){Teminio = is; Index = 0;}
    bool isTermino() const { return Index>=100;}
    void clear(){mPoints.clear();}
    bool isDrawLines()
    {
        return drawLines;
    }
    
    void SetScalar(float s)
    {
        scalar = s;
    }
};


#endif //_CURVABEZIER_HPP_