#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define I_0 1e-12
#define U_T 25.8563e-3

//vypocet rozdilu Ir a Id - tvori funkce pro puleni intervalu
double y(double u0, double r, double u_p)
{
    double diode_current = I_0*(exp(u_p / U_T) -1);
    double resistor_current = (u0-u_p)/r;
    return diode_current - resistor_current;
}
//vypocet napeti v pracovnim bodu diody pomoci puleni intervalu na funkci y
double diode(double u0, double r, double eps)
{
    //interval 0 - U0
    double a = 0;
    double b = u0;
    double u_p = 0;
    //vzdalenost mezi body intervalu
    while(fabs(y(u0, r, a) - y(u0, r, b)) > eps)
    {
        u_p = (a + b) / 2; 
        if(y(u0, r, a) * y(u0, r, u_p) < 0)
        {
            b = u_p;
        }
        else
        {
            a = u_p;
        }   
    }
    return u_p;
}
int main(int argc, char **argv)
{
    //Malo argumentu - min 3
    if(argc<4)
    {
        fprintf(stderr, "Zadano malo argumentu");
        return 1;
    }
    //prevod zadanych hodnot do double
    double u_0 = atof(argv[1]);
    double r = atof(argv[2]);
    double eps = atof(argv[3]);
    //vypocet napeti a proudu v pracovnim bode
    double u_p = diode(u_0,r,eps);
    double i_p = I_0 * (exp(u_p / U_T) -1);
    printf("Up=%g V\n",u_p);
    printf("Ip=%g A\n",i_p);
    return 0;
}
