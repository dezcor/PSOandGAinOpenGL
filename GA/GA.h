#ifndef _GA_H_
#define _GA_H_
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <exception>
#include "../src/Render.hpp"

using namespace std;

namespace GA
{
	class Individuo
	{
	private:
		vector<unsigned int> Cromosoma;
		float Fitness;
		float Objetivo;
		float ProbSel;
		vector<float> valor;

	public:
		//___________Get____________
		unsigned int Getsize();
		float GetFitness();
		float GetObjetivo();
		float GetValor(const int i);
		float GetProbSel();
		vector<unsigned int>& GetCromosoma();
		//___________Set____________
		unsigned int &operator[](const int Index);
		void SetNumeroGenes(const unsigned int size);
		void SetNumeroBitGen(const vector<unsigned int> &NumBitGen);
		void setValor(const unsigned int Index, const float valor);
		void SetObjetivo(const float obj);
		void SetFitness(float fitness);
		void SetProbSel(float pbSel);
		void SetCromosoma(vector<unsigned int>& Cromosoma);

		Individuo();
		~Individuo() {}
	};

	class Poblacion : public Render
	{
	protected:
		unsigned int NumeroDeGenes;
		vector<unsigned int> NumeroBitGet;
		vector<Individuo> individuos;
		vector<Individuo> NewIndividuos;
		vector<unsigned int> idSelect;
		unsigned int SizeCromosoma;
		unsigned int SizePoblacion;
		float ObjetivoMinimo;
		float SumFitness;
		float Pc;
		float Pm;
		int idMejor;
		int MaxError;
		bool inicio = true;
		Render * Objecto;
		glm::vec3 Scalar;
		bool Maximo;
		double  (*funcion)(double * p,const int N);
		static double cuadratica(double * p,const int N)
		{
			double sum = 0;
			for (int i = 0; i < N; i++)
			{
				sum += p[i] * p[i];
			}
			return sum;
		}
		std::vector<float> Limites;
	public:
		float Error;

		//____________Get____________
		unsigned int GetNumGenes();
		unsigned int GetNumBitGet(const unsigned int Index);
		unsigned int GetsizeCromosoma();
		unsigned int GetsizePoblacion();
		int GetidMejor();
		Individuo &operator[](const unsigned int Index);
		bool isMaximo()
		{
			return Maximo;
		}



		//____________Set____________
		void SetNumGenes(const unsigned int NG);
		void SetNumBitGen(const vector<unsigned int> &NumeroBitGet);
		void resizePoblacion(const unsigned int NumeroDeGenes);
		void setProbabilidadCruza(float pc);
		void setProbabilidadMuta(float pm);
		void SetLimites(const std::vector<float> & L)
		{
			Limites = L;
		}
        void setFuncion(double  (*funcion)(double * p,const int N))
		{
			this->funcion = funcion;
		}

		void SetMaximo(bool is)
		{
			Maximo = is;
		}
		Poblacion();
		virtual ~Poblacion() {}



		//_____________________________Algoritmo
		void Inicializa();
		void Ruleta();
		void Cruza();
		void Muta();
		void Evaluacion();
		void Fitness();
		virtual void FuncionObjetivo(Individuo& individuo);
		void ConservarMejor();
		void ActualizarPoblacion();
		void MostrarIndividuo(const unsigned int Index);
		void MostrarPoblacion();



		//********************Decodificar***********************//
		void DecodificarToInt(Individuo& individuo);
		void Run(int64_t Iteraciones, float Error);






		//___________________________End
		/************************Render************/
		void dcRender(const glm::mat4 &m);
        void dcUpdate();
        void SetScalar(const glm::vec3 &S)
        {
            Scalar = S;
        }

        void SetObject(Render * objeto);
	};
}; // namespace GA

#endif //_GA_H_