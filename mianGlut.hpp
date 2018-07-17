#ifndef _APPGLUT_H_
#define _APPGLUT_H_
// Incluir los cabeceros estándar

#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
// Incluir GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Malla.hpp"
#include "CurvaBezier.hpp"
#include "Cube.hpp"
#include "PSO/Enjambre.h"
#include "GA/GA.h"
#include <exception>

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

class App
{
  public:
    GLuint Width;
    GLuint Height;
    GLvoid rezise(uint width,uint height)
    {
        //glViewport(0, 0, width, height);
        Width = width;
        Height = height;
    }
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
    //Cube *cubo;
	GLuint uniform_mytexture;
    GLuint MatrixID;
    static vector<bool> KEY;
    static vector<bool> KEYS;

public:
    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
    {

        // Crear los shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Leer el Vertex Shader desde archivo
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open())
        {
            std::stringstream sstr;
            sstr << VertexShaderStream.rdbuf();
            VertexShaderCode = sstr.str();
            VertexShaderStream.close();
        }
        else
        {
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        // Leer el Fragment Shader desde archivo
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open())
        {
            std::stringstream sstr;
            sstr << FragmentShaderStream.rdbuf();
            FragmentShaderCode = sstr.str();
            FragmentShaderStream.close();
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compilar Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const *VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Revisar Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }

        // Compilar Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const *FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Revisar Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }

        // Vincular el programa por medio del ID
        printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Revisar el programa
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }

        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }
    ~App()
    {
        cleanup();
    }

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
        //delete cubo;
        delete enjambre;
        delete enjambreI;
        delete poblacion;
    }

    void draw()
    {
        glUseProgram(programID);
        glm::mat4 model;
        glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0,2, 0));
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
        glActiveTexture(GL_TEXTURE1);
	    glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
        
        try
        {
            /* code */
            malla->dcRender(GA_vertes);
            poblacion->dcRender(GA_vertes);
            Piso->dcRender(cmPrespetive->GetViewPerpetive() * glm::rotate(glm::mat4(1.0f), glm::radians(float(90)), glm::vec3(1, 0, 0)));
            malla2->dcRender(PSOvertex);
            enjambre->dcRender(PSOvertex);
            malla3->dcRender(PSOIvertex);
            enjambreI->dcRender(PSOIvertex);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        
        
        //if (isProjection)
        //    cubo->dcRender(cmOrtho->GetViewPerpetive() * glm::scale(glm::mat4(1.0f), glm::vec3(0.1)) * model);
        // CurvaBezierCube->dcRender(vertex_transform);
        // curvaBezier->dcRender(vertex_transform);
    }

    void Update()
    {
        static double time = glutGet(GLUT_ELAPSED_TIME);
        static double timet = glutGet(GLUT_ELAPSED_TIME);
        static double speed = 0.05;
        static bool pausa = true;
        if (timet + 1000 / 6.0 < glutGet(GLUT_ELAPSED_TIME) && !pausa)
        {
            //printf("%f\n",glfwGetTime());
            if (enjambre->isReal() ) enjambre->dcUpdate();
            if (enjambreI->isReal()) enjambreI->dcUpdate();
            poblacion->dcUpdate();
            timet = glutGet(GLUT_ELAPSED_TIME);
        }
        if (!enjambre->isReal() && !pausa) enjambre->dcUpdate();
        if (!enjambreI->isReal() && !pausa) enjambreI->dcUpdate();
        if (KEY[GLUT_KEY_LEFT])
        {
            cmPrespetive->Move(CamaraView::Movement::LEFT);
        }
        if (KEY[GLUT_KEY_RIGHT])
        {
            cmPrespetive->Move(CamaraView::Movement::RIGHT);
        }
        if (KEY[GLUT_KEY_UP])
        {
            cmPrespetive->Move(CamaraView::Movement::FORWARD);
        }
        if (KEY[GLUT_KEY_DOWN])
        {
            cmPrespetive->Move(CamaraView::Movement::BACKWARD);
        }

        if(KEYS[27])
        {
            exit(0);
        }

        if(KEYS['R'] | KEYS['r'])
        {
            cleanup();
            InitElementos();
            pausa = true;
        }

        if(KEY[GLUT_KEY_F2])
        {
            pausa = !pausa;
            KEY[GLUT_KEY_F2] = false;
        }
        //isProjection = true;
        if (KEY[GLUT_KEY_F1])
        {
            isProjection = !isProjection;
            KEY[GLUT_KEY_F2] = false;
        }
        // if(isProjection)
        //     glutSetCursor(GLUT_CURSOR_NONE);
        // else
        //     glutSetCursor(GLUT_CURSOR_CROSSHAIR);;
    }

    void InitElementos()
    {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        float Limi = 1.0f;
        cmPrespetive = new CamaraView(glm::vec3(0, 1, 1), glm::radians(45.0f), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        cmOrtho = new CamaraView(glm::vec3(0, 4, 4), -4, 4, 4, -4, 10, -10);
        programID = LoadShaders("cube.v.glsl", "cube.f.glsl");
        MatrixID = glGetUniformLocation(programID, "MVP");
        uniform_mytexture = glGetUniformLocation(programID,"TexCoord");
        malla = new Malla;
        Piso = new Malla;
        malla2  = new Malla;
        malla3 = new Malla;
        Piso->SetMatrizID(MatrixID);
        Piso->dcUpdate();
        //malla uno
        malla->SetNumberLines(50);
        malla->SetLimitesX(-Limi, Limi);
        malla->SetLimitesY(-Limi, Limi);
        malla->SetFuncion(f5);
        malla->SetColor(ColorRGB(1, 1, 0));
        malla->dcUpdate();
        malla->SetMatrizID(MatrixID);

        //malla2
        Limi = 2.0f;
        malla2->SetNumberLines(50);
        malla2->SetLimitesX(-Limi, Limi);
        malla2->SetLimitesY(-Limi, Limi);
        malla2->SetFuncion(F5);
        malla2->SetColor(ColorRGB(1, 0, 1));
        malla2->dcUpdate();
        malla2->SetMatrizID(MatrixID);
        //malla3
        malla3->SetNumberLines(50);
        malla3->SetLimitesX(-Limi, Limi);
        malla3->SetLimitesY(-Limi, Limi);
        malla3->SetFuncion(cuadratica);
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
        enjambre->SetObject(new Cube());
        enjambre->SetOffLimites(false);
        enjambre->SetScalar(malla2->getScalar());
        //PSOI inicializacion del Enjambre de particulas
        enjambreI = new PSO::EnjambrePesoI(200, 2, -Limi, Limi, -Limi * .15, Limi * .15, 2.05, 2.05, 0.7, PSO::Enjambre::Orden::MINIMO);
        enjambreI->setFuncion(cuadratica);
        enjambreI->SetReal(false);
        enjambreI->SetNumberPoints(5);
        enjambreI->SetMatrizID(MatrixID);
        enjambreI->SetObject(new Cube(ColorRGB(1, 0, 0)));
        enjambreI->SetOffLimites(false);
        enjambreI->SetScalar(malla3->getScalar());
        //GA inicializacion de la poblacion
        Limi = 1.0f;
        poblacion = new GA::Poblacion();
        poblacion->setFuncion(f5);
        poblacion->SetMatrizID(MatrixID);
        std::vector<uint> up = {16, 16};
        std::vector<float> limites = {-Limi, Limi};
        poblacion->SetMaximo(true);
        poblacion->SetLimites(limites);
        poblacion->SetNumBitGen(up);
        poblacion->resizePoblacion(100);
        poblacion->setProbabilidadCruza(0.9);
        poblacion->setProbabilidadMuta(0.01);
        //cubo = new Cube();
        poblacion->SetObject(new Cube(ColorRGB(0, 1, 0)));
        poblacion->SetScalar(malla->getScalar());
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
        // Habilidad el test de profundidad
        glEnable(GL_DEPTH_TEST);
        // Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
        glDepthFunc(GL_LESS);
    }

    void moveCamara(int x,int y)
    {
        //printf("%d %d\n", x, y);
        double xpos=Width / 2,ypos= Height / 2;
        static float lastX = x;
        static float lastY = y;
        float xoffset = lastX- xpos;
        float yoffset = ypos - lastY;
        //printf("%f %f\n", xoffset, yoffset); // reversed since y-coordinates range from bottom to top
        lastX = x;
        lastY = y;
        if((xoffset != 0 || yoffset != 0)&& isProjection)
        {
            cmPrespetive->ProcessMouseMovement(xoffset,yoffset);
            glutWarpPointer(Width / 2, Height / 2);
        }
    }

    static void Key(int key, int x, int y)
    {
        //printf("%d %d %d\n", key, x, y);
        KEY[key] = true;
    }

    static void KeyUp(int key, int x, int y)
    {
        //printf("%d %d %d\n", key, x, y);
        KEY[key] = false;
    }

    static void KeyN(u_char key,int x,int y)
    {
        KEYS[key] = true;
    }

    static void KeyNUp(u_char key,int x,int y)
    {
        KEYS[key] = false;
    }
};

glm::vec3 App::Riro = glm::vec3(0);
vector<bool> App::KEY = vector<bool>(255,false);
vector<bool> App::KEYS = vector<bool>(255,false);


App app;
void display()
{
    app.draw();
    glutSwapBuffers();
}

void idle()
{
    app.Update();
    glutPostRedisplay();
}

void reshape(int w,int h)
{
    app.rezise(w,h);
    glViewport(0,0,w,h);
}

void motion(int x,int y)
{
    app.moveCamara(x,y);
}

int mainGLUT(int argc, char* argv[]) {
    app.rezise(770,800);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(app.Height,app.Width);
	glutCreateWindow("PSO and GA in glut");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (!GLEW_VERSION_3_0) {
		fprintf(stderr, "No support for OpenGL 2.0 found\n");
		return 1;
	}

	app.InitElementos();
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutWarpPointer(app.Width/2, app.Height/2);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutIdleFunc(display);
    glutSpecialFunc(app.Key);
    glutKeyboardFunc(app.KeyN);
    glutKeyboardUpFunc(app.KeyNUp);
    glutSpecialUpFunc(app.KeyUp);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(motion);
    glutMotionFunc(motion);
    glutMainLoop();

	return 0;
}

#endif //_CAMARAMALLAGLFW_H_