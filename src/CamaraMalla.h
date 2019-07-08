#ifndef _CAMARAMALLAGLFW_H_
#define _CAMARAMALLAGLFW_H_
// Incluir los cabeceros estándar
#include "Camara.h"

#include "Malla.hpp"
#include "CurvaBezier.hpp"
#include "Cube.hpp"
#include "../PSO/Enjambre.h"
#include "../GA/GA.h"
#include <exception>
#include "GLTexture.hpp"

struct point
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

double cuadratica(double *X, int N)
{
    float x1 = X[0];
    float x2 = X[1];
    double sum1 = 0,sum2 = 0;
    int i;
    for(i=1; i <= 5 ;i++)
    {
        sum1 +=i*cos((i+1)*x1+i);
        sum2 +=i*cos((i+1)*x2+i);
    }

    return sum1*sum2;;
}

double f5(double *X, int N)
{
    float x0 = X[0];
    float x1 = X[1];
    return (x0 * x0 - 10 * cos(2 * M_PIf32 * x0 * x0) + 10) +
           (x1 * x1 - 10 * cos(2 * M_PIf32 * x1 * x1) + 10);
}

double F5(double *Xi, int N)
{
    double sum = 0, sum2 = 0;
    for (int i = 0; i < N; i++)
    {
        sum += Xi[i] * Xi[i];
        sum2 += cos(2 * M_PIf32 * Xi[i]);
    }
    return 20 + exp(1) - 20 * exp(-0.2 * sqrt(sum / N)) -
           exp(sum2 / N);
}

class CamaraMalla : public Camara
{
  public:
  private:
    // Identificar el vertex buffer
    GLuint vertexbuffer;
    GLuint VertexArrayID;
    GLuint programID;
    point Giro;
    Malla *malla;
    Malla * malla2;
    CurvaBezier *curvaBezier;
    CurvaBezier *CurvaBezierCube;
    CamaraView *cmPrespetive;
    CamaraView *cmOrtho;
    static glm::vec3 Riro;
    bool isProjection = true;
    PSO::Enjambre *enjambre;
    PSO::Enjambre *enjambreI;
    GA::Poblacion *poblacion;
    Malla *Piso;
    Malla * malla3;
    Cube *cubo;
    Texture cuboGA,CuboPSO,CuboPSOI;
	GLuint uniform_mytexture;
    static vector<bool> KEY;
    void cleanup()
    {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);
        delete malla;
        delete malla2;
        delete malla3;
        delete Piso;
        // delete CurvaBezierCube;
        // delete curvaBezier;
        delete cmPrespetive;
        delete cmOrtho;
        std::cout << "\n\nPSO*************************" << std::endl;
        //enjambre->MostarEjambre();
        enjambre->MostrarMejorParticula();
        std::cout << "\n\nPSOInercia*****************" << std::endl;
        // enjambreI->MostarEjambre();
        enjambreI->MostrarMejorParticula();
        std::cout << "\n\nGA*************************" << std::endl;
        //poblacion->MostrarPoblacion();
        std::cout << "Mejor Individuo :" << poblacion->GetidMejor() << std::endl;
        poblacion->MostrarIndividuo(poblacion->GetidMejor());
        delete cubo;
        delete enjambre;
        delete enjambreI;
        delete poblacion;
    }

    void draw()
    {
        glUseProgram(programID);
        glm::mat4 model;
        glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0, 2, 0));
        model = glm::rotate(glm::mat4(1.0f), glm::radians(Riro.x), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(Riro.y), glm::vec3(0, 1, 0)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Riro.z), glm::vec3(0, 0, 1));

        glm::mat4 GA_vertes = cmPrespetive->GetViewPerpetive() * tras * model;
        tras = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 2, 0));
        glm::mat4 PSOIvertex = cmPrespetive->GetViewPerpetive() * tras * model;
        tras = glm::translate(glm::mat4(1.0f), glm::vec3(6, 2, 0));
        glm::mat4 PSOvertex = cmPrespetive->GetViewPerpetive() * tras * model;
        glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw Objetos
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(uniform_mytexture, /*GL_TEXTURE*/ 0);

        try
        {
            /* code */
            cuboGA.Bin();
            malla->dcRender(GA_vertes);
            poblacion->dcRender(GA_vertes);
            CuboPSO.Bin();
            Piso->dcRender(cmPrespetive->GetViewPerpetive() * glm::rotate(glm::mat4(1.0f), glm::radians(float(90)), glm::vec3(1, 0, 0)));
            malla3->dcRender(PSOIvertex);
            enjambre->dcRender(PSOIvertex);
            CuboPSOI.Bin();
            malla2->dcRender(PSOvertex);
            enjambreI->dcRender(PSOvertex);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        
        
        
        if (isProjection)
            cubo->dcRender(cmOrtho->GetViewPerpetive() * glm::scale(glm::mat4(1.0f), glm::vec3(0.1)) * model);
        // CurvaBezierCube->dcRender(vertex_transform);
        // curvaBezier->dcRender(vertex_transform);
    }

    void Update()
    {
        static double time = glfwGetTime();
        static double timet = glfwGetTime();
        // static double timeG = glfwGetTime();
        static double speed = 0.05;
        static bool pausa = true;
        if (timet + 1 / 6.0 < glfwGetTime() && !pausa)
        {
            //printf("%f\n",glfwGetTime());
            if (enjambre->isReal() ) enjambre->dcUpdate();
            if (enjambreI->isReal()) enjambreI->dcUpdate();
            poblacion->dcUpdate();
            timet = glfwGetTime();
        }

        // if (timeG + 1 / 6.0 * 5 < glfwGetTime() && !pausa)
        // {
            // //printf("%f\n",glfwGetTime());
            // poblacion->dcUpdate();
            // timeG = glfwGetTime();
        // }

        if (!enjambre->isReal() && !pausa) enjambre->dcUpdate();
        if (!enjambreI->isReal() && !pausa) enjambreI->dcUpdate();
        double xpos=0,ypos=0;
        const float lastX = Width/2;
        const float lastY = Height/2;
        glfwGetCursorPos(window,&xpos,&ypos);
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

        if((xoffset != 0 || yoffset != 0)&& isProjection)
        {
            cmPrespetive->ProcessMouseMovement(xoffset,yoffset);
            glfwSetCursorPos(window,Width/2,Height/2);
        }
        if (glfwGetKey(window,GLFW_KEY_LEFT))
        {
            cmPrespetive->Move(CamaraView::Movement::LEFT);
        }
        if (glfwGetKey(window,GLFW_KEY_RIGHT))
        {
            cmPrespetive->Move(CamaraView::Movement::RIGHT);
        }
        if (glfwGetKey(window,GLFW_KEY_UP))
        {
            cmPrespetive->Move(CamaraView::Movement::FORWARD);
        }
        if (glfwGetKey(window,GLFW_KEY_DOWN))
        {
            cmPrespetive->Move(CamaraView::Movement::BACKWARD);
        }

        if (KEY[GLFW_KEY_KP_8])
        {
            if (this->Riro.x > 360)
                Riro.x = 0;
            else
                this->Riro.x++;
        }
        if (KEY[GLFW_KEY_KP_2])
        {
            if (this->Riro.x < 0)
                Riro.x = 360;
            else
                this->Riro.x--;
        }
        if (KEY[GLFW_KEY_KP_4])
        {
            if (this->Riro.z > 360)
                Riro.z = 0;
            else
                this->Riro.z++;
        }
        if (KEY[GLFW_KEY_KP_6])
        {
            if (this->Riro.z < 0)
                Riro.z = 360;
            else
                this->Riro.z--;
        }
        if (KEY[GLFW_KEY_KP_1])
        {
            if (this->Riro.y > 360)
                Riro.y = 0;
            else
                this->Riro.y++;
        }
        if (KEY[GLFW_KEY_KP_3])
        {
            if (this->Riro.y < 0)
                Riro.y = 360;
            else
                this->Riro.y--;
        }
        if (KEY[GLFW_KEY_SPACE])
        {
            time = glfwGetTime();
            pausa = !pausa;
        }
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != Width || height != height)
        {
            glViewport(0, 0, width, height);
            Width = width;
            Height = height;
        }
        //isProjection = true;
        if (KEY[GLFW_KEY_F1])
        {
            if (glfwGetTime() - time > 0.2)
            {
                time = glfwGetTime();
                isProjection = !isProjection;
                glfwSetCursorPos(window,width/2,height/2);
            }
        }
        if(isProjection)
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }

    void InitElementos()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        float Limi = 2.0f;
        cmPrespetive = new CamaraView(glm::vec3(0, 1, 1), glm::radians(45.0f), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        cmOrtho = new CamaraView(glm::vec3(0, 4, 4), -4, 4, 4, -4, 10, -10);
        programID = LoadShaders("res/cube.v.glsl", "res/cube.f.glsl");
        MatrixID = glGetUniformLocation(programID, "MVP");
        uniform_mytexture = glGetUniformLocation(programID,"TexCoord");
        cuboGA.load("res/GA.jpg");
        CuboPSO.load("res/res_texture.png");
        CuboPSOI.load("res/PSOI.jpg");
        malla = new Malla;
        Piso = new Malla;
        malla2  = new Malla;
        malla3 = new Malla;
        Piso->SetMatrizID(MatrixID);
        Piso->dcUpdate();
        //malla uno
        malla->SetNumberLines(100);
        malla->SetLimitesX(-Limi, Limi);
        malla->SetLimitesY(-Limi, Limi);
        malla->SetFuncion(cuadratica);
        malla->SetColor(ColorRGB(1, 1, 0));
        malla->dcUpdate();
        malla->SetMatrizID(MatrixID);

        //malla2
        Limi = 2.0f;
        malla2->SetNumberLines(50);
        malla2->SetLimitesX(-Limi, Limi);
        malla2->SetLimitesY(-Limi, Limi);
        malla2->SetFuncion(cuadratica);
        malla2->SetColor(ColorRGB(1, 0, 1));
        malla2->dcUpdate();
        malla2->SetMatrizID(MatrixID);
        //malla3
        malla3->SetNumberLines(50);
        malla3->SetLimitesX(-Limi, Limi);
        malla3->SetLimitesY(-Limi, Limi);
        malla3->SetFuncion(F5);
        malla3->SetColor(ColorRGB(0, 1, 1));
        malla3->dcUpdate();
        malla3->SetMatrizID(MatrixID);
        Riro = glm::vec3(360 - 90, 0, 0);
        //PSO inicializacion del Enjambre de particulas
        enjambre = new PSO::Enjambre(100, 2, -Limi, Limi, -Limi * .15, Limi * .15, 2, 2, 0.7, PSO::Enjambre::Orden::MINIMO);
        enjambre->setFuncion(F5);
        enjambre->SetReal(false);
        enjambre->SetNumberPoints(5);
        enjambre->SetMatrizID(MatrixID);
        enjambre->SetObject(new Cube(ColorRGB(1, 1, 0)));
        enjambre->SetOffLimites(false);
        enjambre->SetScalar(malla3->getScalar());
        //PSOI inicializacion del Enjambre de particulas
        enjambreI = new PSO::EnjambrePesoI(200, 2, -Limi, Limi, -Limi * .15, Limi * .15, 2.05, 2.05, 0.7, PSO::Enjambre::Orden::MAXIMO);
        enjambreI->setFuncion(cuadratica);
        enjambreI->SetReal(false);
        enjambreI->SetNumberPoints(5);
        enjambreI->SetMatrizID(MatrixID);
        enjambreI->SetObject(new Cube(ColorRGB(1, 0, 0)));
        enjambreI->SetOffLimites(false);
        enjambreI->SetScalar(malla2->getScalar());
        //GA inicializacion de la poblacion
        Limi = 2.0f;
        poblacion = new GA::Poblacion();
        poblacion->setFuncion(cuadratica);
        poblacion->SetMatrizID(MatrixID);
        std::vector<GLuint> up = {16, 16};
        std::vector<float> limites = {-Limi, Limi};
        poblacion->SetMaximo(true);
        poblacion->SetLimites(limites);
        poblacion->SetNumBitGen(up);
        poblacion->resizePoblacion(200);
        poblacion->setProbabilidadCruza(0.5);
        poblacion->setProbabilidadMuta(0.3);
        cubo = new Cube();
        poblacion->SetObject(new Cube(ColorRGB(0, 1, 0)));
        poblacion->SetScalar(malla->getScalar());
        cubo->SetMatrizID(MatrixID);
        // curvaBezier = new CurvaBezier;
        // curvaBezier->SetScalar(0.01);
        // CurvaBezierCube = new CurvaBezier;
        // CurvaBezierCube->SetScalar(0.01);
        // vector<glm::vec3> points ={{0,0,0},{0.7,0.5,0},{-1,1,0},{1.4,1.5,0},{0.2,2,0},{-1,1,1}};
        // curvaBezier->SetPoints(points);
        // curvaBezier->dcUpdate();
        // curvaBezier->SetMatrizID(MatrixID);
        // curvaBezier->SetObject(new Cube);
        // curvaBezier->SetDrawLines(true);
        // vector<glm::vec3> points2 ={{0,0,0},{0.4,0.5,-1},{-1,1,2},{2,3,1},{0.2,2,0},{-1,1,1}};
        // CurvaBezierCube->SetPoints(points2);
        // CurvaBezierCube->dcUpdate();
        // CurvaBezierCube->SetMatrizID(MatrixID);
        // CurvaBezierCube->SetObject(new Cube);
        // CurvaBezierCube->SetDrawLines(true);
        glfwSetKeyCallback(window,Key);
        // Habilidad el test de profundidad
        glEnable(GL_DEPTH_TEST);
        // Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
        glDepthFunc(GL_LESS);
    }

    static void Key(GLFWwindow *window, int key, int estado, int x, int y)
    {
        //printf("%d %d %d %d\n", key, estado, x, y);
        if(x == GLFW_PRESS)
            KEY[key] = true;
        else if(x == 0)
        {
            KEY[key] = false;
        }
    }
};

glm::vec3 CamaraMalla::Riro = glm::vec3(0);
vector<bool> CamaraMalla::KEY = vector<bool>(350,false);
#endif //_CAMARAMALLAGLFW_H_