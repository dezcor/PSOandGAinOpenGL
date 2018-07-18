#include "Enjambre.h"

namespace PSO
{
Enjambre::Enjambre()
{
    pEnj = NULL;
    Pgid = 0;
    NumeroParticulas = 0;
    NumeroParametrosPorParticula = 0;
    LimiteSuperiorXi = 100;
    LimitInferiorXi = -100;
    C1 = 2.0f;
    C2 = 2.0f;
    LimitInferiorVi = -100.20f;
    LimiteSuperiorVi = 100.20f;
    fun = Maximo;
    funcion = cuadratica;
    offLimites = false;
    mIsReal = true;
}

Enjambre::Enjambre(const unsigned int NP,
                   const unsigned char NPP,
                   const double LIXi,
                   const double LSXi,
                   const double LIVi,
                   const double LSVi,
                   const double C1 = 2,
                   const double C2 = 2,
                   const double W = 0.6,
                   const Enjambre::Orden in = Enjambre::Orden::MINIMO) : Enjambre()
{
    NumeroParametrosPorParticula = NPP;
    NumeroParticulas = NP;
    NewParticulas();
    LimiteSuperiorXi = LSXi;
    LimitInferiorXi = LIXi;
    LimitInferiorVi = LIVi;
    LimiteSuperiorVi = LSVi;
    this->C1 = C1;
    this->C2 = C2;
    this->W = W;
    if (in == Orden::MINIMO)
        fun = Minimo;
    else
        fun = Maximo;
}

Enjambre::~Enjambre()
{
    DeleteParticulas();
}

void Enjambre::NewParticulas()
{
    DeleteParticulas();
    if (pEnj == NULL)
    {
        pEnj = new Particula[NumeroParticulas];
        unsigned int i;
        for (i = 0; i < NumeroParticulas; i++)
        {
            pEnj[i].Xi = new double[NumeroParametrosPorParticula];
            pEnj[i].Vi = new double[NumeroParametrosPorParticula];
            pEnj[i].Pi = new double[NumeroParametrosPorParticula];
        }
    }
}

void Enjambre::DeleteParticulas()
{
    if (pEnj == NULL)
        return;
    for (int i = 0; i < NumeroParticulas; i++)
    {
        delete[] pEnj[i].Xi;
        delete[] pEnj[i].Vi;
        delete[] pEnj[i].Pi;
    }
    delete[] pEnj;
    pEnj = NULL;
}

void Enjambre::InicializarEnjambre()
{
    unsigned int i;
    int d;
    if (pEnj == NULL)
        return;
    for (i = 0; i < NumeroParticulas; i++)
    {
        for (d = 0; d < NumeroParametrosPorParticula; d++)
        {
            //Generar un numero aleatorio en el rango del espacio de busqueda.
            pEnj[i].Xi[d] = (double)rand() / RAND_MAX * (LimiteSuperiorXi - LimitInferiorXi) + LimitInferiorXi;
            pEnj[i].Vi[d] = 0;
            pEnj[i].Pi[d] = pEnj[i].Xi[d];
        }
    }
}

void Enjambre::EvaluarEjambreI()
{
    unsigned int i;
    int d;
    for (i = 0; i < NumeroParticulas; i++)
    {
        pEnj[i].Fxi = funcion(pEnj[i].Xi, NumeroParametrosPorParticula);
        /*Pi <= Xi*/
        for (d = 0; d < NumeroParametrosPorParticula; d++)
        {
            pEnj[i].Pi[d] = pEnj[i].Xi[d];
        }
        /*Fpi = Fxi*/
        pEnj[i].Fpi = pEnj[i].Fxi;
    }

    for (i = 0U; i < NumeroParticulas; i++)
    {
        if (fun(pEnj[i].Fpi, pEnj[Pgid].Fpi))
            Pgid = i;
    }
}

void Enjambre::EvaluarEjambre()
{
    unsigned int i;
    int d;
    for (i = 0U; i < NumeroParticulas; i++)
    {
        pEnj[i].Fxi = funcion(pEnj[i].Xi, NumeroParametrosPorParticula);

        if (fun(pEnj[i].Fxi, pEnj[i].Fpi))
        {
            /*Pi <= Xi*/
            for (d = 0; d < NumeroParametrosPorParticula; d++)
            {
                pEnj[i].Pi[d] = pEnj[i].Xi[d];
            }
            /*Fpi = Fxi*/
            pEnj[i].Fpi = pEnj[i].Fxi;
        }

        if (fun(pEnj[i].Fpi, pEnj[Pgid].Fpi))
            Pgid = i;
    }
}

void Enjambre::ActualizarVelocidad()
{
    unsigned int i;
    int d;
    double E1, E2, aux;
    for (i = 0; i < NumeroParticulas; i++)
    {
        for (d = 0; d < NumeroParametrosPorParticula; d++)
        {
            E1 = (double)rand() / RAND_MAX;
            E2 = (double)rand() / RAND_MAX;
            aux = pEnj[i].Vi[d] + C1 * E1 * (pEnj[i].Pi[d] - pEnj[i].Xi[d]) + C2 * E2 * (pEnj[Pgid].Pi[d] - pEnj[i].Xi[d]);

            if (!offLimites)
            {
                if (aux > LimiteSuperiorVi)
                {
                    pEnj[i].Vi[d] = LimiteSuperiorVi;
                    continue;
                }
                else if (aux < LimitInferiorVi)
                {
                    pEnj[i].Vi[d] = LimitInferiorVi;
                    continue;
                }
            }
            pEnj[i].Vi[d] = aux;
        }
    }
}

void Enjambre::ActualizarPosicion()
{
    unsigned int i;
    int d;
    for (i = 0; i < NumeroParticulas; i++)
    {
        for (d = 0; d < NumeroParametrosPorParticula; d++)
        {
            pEnj[i].Xi[d] += pEnj[i].Vi[d];
            if (pEnj[i].Xi[d] >= LimiteSuperiorXi)
                pEnj[i].Xi[d] = LimiteSuperiorXi;
            else if (pEnj[i].Xi[d] <= LimitInferiorXi)
                pEnj[i].Xi[d] = LimitInferiorXi;
        }
    }
}

void Enjambre::MostarParticula(int Index)
{
    int d;
    printf("\nX: ");
    for (d = 0; d < NumeroParametrosPorParticula; d++)
    {
        printf("%f\t", pEnj[Index].Xi[d]);
    }
    printf("\nV: ");
    for (d = 0; d < NumeroParametrosPorParticula; d++)
    {
        printf("%f\t", pEnj[Index].Vi[d]);
    }
    printf("\nP: ");
    for (d = 0; d < NumeroParametrosPorParticula; d++)
    {
        printf("%f\t", pEnj[Index].Pi[d]);
    }
    printf("\nFxi: %f", pEnj[Index].Fxi);
    printf("\nFpi: %f\n\n", pEnj[Index].Fpi);
}

void Enjambre::MostarEjambre()
{
    unsigned int i;
    for (i = 0; i < NumeroParticulas; i++)
    {
        printf("Particula: %d \n", i);
        MostarParticula(i);
    }
}
void Enjambre::MostrarMejorParticula()
{
    printf("Mejor Particula: %d\n", Pgid);
    MostarParticula(this->Pgid);
}

int Enjambre::Run(const int Iteraciones = 1000, Enjambre::PRINTF mostrar = Enjambre::PRINTF::NOMOSTRARPARTICULA, const double Error = 0.0001)
{
    /*Inicializar Enjambre*/
    this->InicializarEnjambre();
    /*Evaluar Enjambre*/
    this->EvaluarEjambreI();
    /*Mostrar Enjambre*/
    if (mostrar == PRINTF::MOSTRARPARTICULAS)
        this->MostarEjambre();
    int Iteracion = 0;
    while (Iteracion < Iteraciones && pEnj[Pgid].Fpi > Error /**/)
    {
        /*ActualizarVelocidad*/
        this->ActualizarVelocidad();
        /*ActualizarPosicion*/
        this->ActualizarPosicion();
        /*EvaluarEnjambre*/
        this->EvaluarEjambre();
        /*Mostrar Enjambre*/
        if (mostrar == PRINTF::MOSTRARPARTICULAS)
            this->MostarEjambre();
        Iteracion++;
    }
    return Iteracion;
}

void Enjambre::setFuncion(double (*funcion)(double *p, const int N))
{
    this->funcion = funcion;
}

double Enjambre::getError()
{
    return pEnj[Pgid].Fpi;
}

bool Enjambre::Maximo(double A, double B)
{
    return A > B;
}

bool Enjambre::Minimo(double A, double B)
{
    return A < B;
}

double Enjambre::cuadratica(double *pEnj, const int N)
{
    double sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += pEnj[i] * pEnj[i];
    }
    return sum;
}

void Enjambre::SetOffLimites(bool is)
{
    offLimites = is;
}

bool Enjambre::isOffLimites()
{
    return offLimites;
}

/*********Render**********/
void Enjambre::dcRender(const glm::mat4 &m)
{
    if (inicio)
        return;
    if (isReal())
    {
        glm::mat4 scalar = glm::scale(glm::mat4(1.0f), glm::vec3(0.02));
        glm::mat4 tras, mvp;
        float medio = (Scalar.x - Scalar.y) / 2 * Scalar.z;
        for (size_t i = 0; i < NumeroParticulas; i++)
        {
            glm::vec3 X = glm::vec3(pEnj[i].Xi[0], pEnj[i].Xi[1], pEnj[i].Fxi * Scalar.z - medio);
            tras = glm::translate(glm::mat4(1.0f), X);
            mvp = m * tras * scalar;
            Objeto->dcRender(mvp);
        }
    }
    else
    {
        // glm::mat4 sScalar = glm::scale(glm::mat4(1.0f), glm::vec3(0.02));
        // glm::mat4 tras, mvp;
        for (uint i = 0; i < curva.size(); i++)
        {
            // tras = glm::translate(glm::mat4(1.0f), curva[i].GetPoint(0));
            // mvp = m * tras * sScalar;
            // Objeto->dcRender(mvp);
            curva[i].dcRender(m);
        }
    }
}

void Enjambre::dcUpdate()
{
    int i = 0;
    if (isReal())
    {
        if (inicio)
        {
            /*Inicializar Enjambre*/
            this->InicializarEnjambre();
            /*Evaluar Enjambre*/
            this->EvaluarEjambreI();
            inicio = false;
        }
        else
        {
            /*ActualizarVelocidad*/
            this->ActualizarVelocidad();
            /*ActualizarPosicion*/
            this->ActualizarPosicion();
            /*EvaluarEnjambre*/
            this->EvaluarEjambre();
            /*Mostrar Enjambre*/
        }
    }
    else
    {
        Avanzar();
        if (mTermino)
        {
            mTermino = false;
            if (inicio)
            {
                /*Inicializar Enjambre*/
                this->InicializarEnjambre();
                /*Evaluar Enjambre*/
                this->EvaluarEjambreI();
                inicio = false;
                curva.resize(NumeroParticulas);
                for (uint j = 0; j < NumeroParticulas; j++)
                {
                    curva[j].SetDelete(false);
                    curva[j].SetObject(Objeto);
                    curva[j].SetScalar(0.02);
                }
            }
            float medio = (Scalar.x + Scalar.y) / 2 * Scalar.z;
            for (uint j = 0; j < NumeroParticulas; j++)
            {
                curva[j].clear();
                double X[3] = {pEnj[j].Xi[0], pEnj[j].Xi[1], pEnj[j].Fxi* Scalar.z - medio};
                curva[j].SetPoint(X, NumeroParametrosPorParticula + 1);
            }
            i++;
            while (i < mNumberPoints)
            {
                /*ActualizarVelocidad*/
                this->ActualizarVelocidad();
                /*ActualizarPosicion*/
                this->ActualizarPosicion();
                /*EvaluarEnjambre*/
                this->EvaluarEjambre();
                /*Mostrar Enjambre*/
                for (uint j = 0; j < NumeroParticulas; j++)
                {
                    double X[3] = {pEnj[j].Xi[0], pEnj[j].Xi[1], pEnj[j].Fxi* Scalar.z - medio};
                    curva[j].SetPoint(X, NumeroParametrosPorParticula + 1);
                }
                i++;
            }
            for (uint j = 0; j < NumeroParticulas; j++)
            {
                curva[j].GenerarCuba();
                curva[j].dcUpdate();
            }
        }
    }
}

void Enjambre::SetObject(Render *objeto)
{
    Objeto = objeto;
    Objeto->dcUpdate();
}

void Enjambre::Avanzar()
{
    if (inicio == true)
    {
        mTermino = true;
    }
    else
    {

        mTermino = curva[0].isTermino();
        for (uint j = 0; j < NumeroParticulas; j++)
        {
            curva[j].dcUpdate();
        }
        if(mTermino)
        {
            for (uint j = 0; j < NumeroParticulas; j++)
            {
                curva[j].SetTermino(false);
            }
        }
    }
}
//*******************************************************
//*******************************************************

EnjambrePesoI::EnjambrePesoI()
{
    pEnj = NULL;
    Pgid = 0;
    NumeroParticulas = 0;
    NumeroParametrosPorParticula = 0;
    LimiteSuperiorXi = 100;
    LimitInferiorXi = -100;
    C1 = 2.0f;
    C2 = 2.0f;
    LimitInferiorVi = -100.20f;
    LimiteSuperiorVi = 100.20f;
    fun = Maximo;
    funcion = cuadratica;
    offLimites = false;
    mIsReal = true;
}

EnjambrePesoI::EnjambrePesoI(const unsigned int NP,
                             const unsigned char NPP,
                             const double LIXi,
                             const double LSXi,
                             const double LIVi,
                             const double LSVi,
                             const double C1 = 2.05,
                             const double C2 = 2.05,
                             const double W = 0.6,
                             const Enjambre::Orden in = Enjambre::Orden::MINIMO) : EnjambrePesoI()
{
    NumeroParametrosPorParticula = NPP;
    NumeroParticulas = NP;
    NewParticulas();
    LimiteSuperiorXi = LSXi;
    LimitInferiorXi = LIXi;
    LimitInferiorVi = LIVi;
    LimiteSuperiorVi = LSVi;
    this->C1 = C1;
    this->C2 = C2;
    this->W = W;
    if (in == Orden::MINIMO)
        fun = Minimo;
    else
        fun = Maximo;
}

EnjambrePesoI::~EnjambrePesoI()
{
    DeleteParticulas();
}

void EnjambrePesoI::ActualizarVelocidad()
{
    uint i;
    u_char d;
    double E1, E2, aux;
    for (i = 0; i < NumeroParticulas; i++)
    {
        for (d = 0; d < NumeroParametrosPorParticula; d++)
        {
            E1 = (double)rand() / RAND_MAX;
            E2 = (double)rand() / RAND_MAX;
            aux = W * pEnj[i].Vi[d] + C1 * E1 * (pEnj[i].Pi[d] - pEnj[i].Xi[d]) + C2 * E2 * (pEnj[Pgid].Pi[d] - pEnj[i].Xi[d]);

            if (!offLimites)
            {
                if (aux > LimiteSuperiorVi)
                {
                    pEnj[i].Vi[d] = LimiteSuperiorVi;
                    continue;
                }
                else if (aux < LimitInferiorVi)
                {
                    pEnj[i].Vi[d] = LimitInferiorVi;
                    continue;
                }
            }
            pEnj[i].Vi[d] = aux;
        }
    }
}
}; // namespace PSO