#ifndef _Enjambre_H_
#define _Enjambre_H_

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include "../Render.hpp"
#include "../CurvaBezier.hpp"

using namespace std;
namespace PSO 
{
    typedef struct Particula
    {
        double *Xi;
        double *Vi;
        double *Pi;
        double Fxi;
        double Fpi;
        Particula()
        {
            Xi = NULL;
            Vi = NULL;
            Pi = NULL;
            Fxi=0;
            Fpi=0;
        }
    }PARTICULA;

    class Enjambre : public Render
    {
    protected:
        Enjambre();
        unsigned int NumeroParticulas;
        unsigned char NumeroParametrosPorParticula; 
        double LimitInferiorXi;
        double LimiteSuperiorXi;
        double LimiteSuperiorVi;
        double LimitInferiorVi;
        double C1;
        double C2;
        double W;
        unsigned int Pgid;
        PARTICULA* pEnj;
        void DeleteParticulas();
        void NewParticulas();
        bool (*fun)(double A,double B);
        double  (*funcion)(double * p,const int N);
        bool offLimites;
        bool inicio = true;
        Render * Objeto;
        glm::vec3 Scalar;
        bool mIsReal;
        bool mTermino = false;
        //Curvas de bezier 
        std::vector<CurvaBezier> curva;
        unsigned int mNumberPoints; 
    public:
        enum class Orden
        {
            MINIMO,
            MAXIMO,
        };
        enum class PRINTF
        {
            MOSTRARPARTICULAS,
            NOMOSTRARPARTICULA
        };
        Enjambre(const unsigned int NumeroParticulas,
                const unsigned char NumeroParametrosPorParticula, 
                const double LimitInferiorXi,
                const double LimiteSuperiorXi,
                const double LimitInferiorVi,
                const double LimiteSuperiorVi,
                const double C1,
                const double C2,
                const double W,
                const Orden in);
        virtual ~Enjambre();
        void InicializarEnjambre();
        void EvaluarEjambre();
        virtual void ActualizarVelocidad();
        void ActualizarPosicion();
        void EvaluarEjambreI();
        void MostarParticula(int Index);
        void MostarEjambre();
        void MostrarMejorParticula();
        void setFuncion(double  (*funcion)(double * p,const int N));
        double getError();
        static bool Maximo(double A,double B);
        static bool Minimo(double A,double B);
        static double cuadratica(double * p,const int N);
        Particula getMejorParticula();
        void SetOffLimites(bool is);
        bool isOffLimites();
        int Run(const int Iteraciones,PRINTF mostrar,const double Error);

        /*********Render**********/
        void dcRender(const glm::mat4 &m);
        void dcUpdate();
        void SetScalar(const glm::vec3 &S)
        {
            Scalar = S;
        }

        void SetObject(Render * objeto);

        //Set Curvas
        void SetNumberPoints(const uint N){ mNumberPoints = N;}
        void SetReal(bool is){ mIsReal = is;}

        //Get
        inline int GetNumberPoints() const {return mNumberPoints;}
        inline bool isReal() const {return  mIsReal;}
        void Avanzar();
    };

    class EnjambrePesoI: public Enjambre
    {
        protected:
        EnjambrePesoI();
        public:
        EnjambrePesoI(const unsigned int NumeroParticulas,
                const unsigned char NumeroParametrosPorParticula, 
                const double LimitInferiorXi,
                const double LimiteSuperiorXi,
                const double LimitInferiorVi,
                const double LimiteSuperiorVi,
                const double C1,
                const double C2,
                const double W,
                const Orden in);
        ~EnjambrePesoI();
        void ActualizarVelocidad();
    };
};
#endif //_Enjambre_H_