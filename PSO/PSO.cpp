//Numero de particulas
#include "PSO.hpp"

double f0(double * pEnj,const int N)
{
    double sum=0;
    for(int i = 0;i < N; i++)
    {
        sum += pEnj[i]*pEnj[i];
    }
    return sum;
}

double f1(double * pEnj,const int N)
{
    double sum=0,x=0,x1=0,aux=0;
    int Nu=N-1;
    for(int i = 0;i < Nu; i++)
    {
        x = pEnj[i];
        x1 = pEnj[i+1];
        aux= x1-x*x;
        sum += 100*aux*aux + (x-1)*(x-1);
    }
    return sum;
}

double f2(double * pEnj,const int N)
{
    double sum=0,x=0;
    const double pi = 3.14159265359;
    for(int i = 0;i < N; i++)
    {
        x = pEnj[i];
        sum += x*x- 10*cos(2*pi*x)+10;
    }
    return sum;
}

double f3(double * pEnj,const int N)
{
    double sum=0,x=0,aux=0;
    double SUM=f0(pEnj,N);
    for(int i = 0;i < N; i++)
    {
        x = pEnj[i];
        sum*=cos(x/sqrt(i+1));
    }
    return SUM/4000-sum+1;
}

double f6(double * pEnj,const int N)
{
    double sum=0,x=0,aux=0;
    for(int i = 0;i < N; i++)
    {
        x = pEnj[i];
        x = floor(x+1.0/2.0);
        sum+=x*x;
    }
    return sum;
}

double f16(double * pEnj,const int N)
{
    double x0=pEnj[0];
    double x1=pEnj[1];
    return 4*x0*x0-2.1*x0*x0*x0*x0+1.0/3.0*x0*x0*x0*x0*x0*x0+
            x0*x1 - 4*x1*x1+4*x1*x1*x1*x1;
}

double f7(double * pEnj,const int N)
{
    double sum=0,aux=0;
    for(int i = 0;i < N; i++)
    {
        aux= pEnj[i];
        sum += (i+1)*aux*aux*aux*aux;
    }
    return sum+rand()/(double)RAND_MAX;
}

double f8(double * pEnj,const int N)
{
    double sum=0,x=0;
    for(int i = 0;i < N; i++)
    {
        x= pEnj[i];
        sum += -x*sin(sqrt(fabs(x)));
    }
    return sum;
}

double f4(double *pEnj,const int N)
{
    double sum=fabs(pEnj[0]),x=0;
    for(int i = 0;i < N; i++)
    {
        if(fabs(pEnj[i])>sum)
        {
            sum=fabs(pEnj[i]);
        }
    }
    return sum;
}