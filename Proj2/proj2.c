/**************************************************/
/*                 Josef Kotoun                   */
/*                   xkotou06                     */
/*                 2. projekt IZP                 */
/*                Iterační výpočty                */
/*  Výpočet pravcovního bodu diody pomocí bisekce */
/*                                                */
/**************************************************/  
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define I_0 1e-12
#define U_T 25.8563e-3
#define MAX_ITERATIONS 1000

enum ERRORS {ARGS_ERROR = 1, FORMAT_ERROR, EPS_ERROR, DATA_ERROR};

//vypocet rozdilu Id a Ir 
double Id_Ir_difference(double u0, double r, double u_p)
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
    int i = 0;
    //vzdalenost mezi body intervalu
     u_p = (a + b) / 2; 
    while(fabs(a-b) >= eps)
    {
        u_p = (a + b) / 2; 
        //vyber spravneho intervalu
        if(Id_Ir_difference(u0, r, a) * Id_Ir_difference(u0, r, u_p) < 0)
        {
            b = u_p;
        }
        else
        {
            a = u_p;
        }  
        //omezeni iteraci kvuli zacykleni pri prilis velke presnosti
        if(i>MAX_ITERATIONS)
        {
            return u_p;
        } 
        i++;
    }
    return u_p;
}
int main(int argc, char **argv)
{
    //Malo argumentu - min 3
    if(argc<4)
    {
        fprintf(stderr, "error: invalid arguments");
        return ARGS_ERROR;
    }
    //prevod zadanych hodnot do double
    char *p_u0_end;
    double u_0 = strtod(argv[1],&p_u0_end);
    char *p_r_end;
    double r = strtod(argv[2], &p_r_end);
    char *p_eps_end;
    double eps = strtod(argv[3],&p_eps_end);
    //osetreni, jestli byl vstup validni realne cislo
    if(*p_u0_end != '\0' || *p_r_end != '\0' ||  *p_eps_end != '\0')
    {
        fprintf(stderr, "error: invalid arguments\n");
        return FORMAT_ERROR;
    }
    //osetreni zaporneho napeti a odporu
    if(r < 0 || u_0 < 0 ||eps < 0)
    {
        fprintf(stderr, "error: invalid arguments\n");
        return DATA_ERROR;
    }
    //vypocet napeti a proudu v pracovnim bode
    double u_p = diode(u_0,r,eps);
    double i_p = I_0 * (exp(u_p / U_T) -1);
    printf("Up=%g V\n",u_p);
    printf("Ip=%g A\n",i_p);
    return 0;
}