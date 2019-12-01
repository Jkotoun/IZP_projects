/**************************************************/
/*                 Josef Kotoun                   */
/*                   xkotou06                     */
/*                 2. projekt IZP                 */
/*                Iterační výpočty                */
/*  Výpočet pracovního bodu diody pomocí bisekce  */
/*                                                */
/**************************************************/  
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define I_0 1e-12
#define U_T 25.8563e-3

enum ERRORS {ARGS_ERROR = 1, FORMAT_ERROR, EPS_ERROR, DATA_ERROR};

//vypocet rozdilu Id a Ir 
double Id_Ir_difference(double u0, double r, double u_p)
{
    double diode_current = I_0*(exp(u_p / U_T) -1);
    double resistor_current = (u0-u_p)/r;
    return diode_current - resistor_current;
}
//vypocet napeti v pracovnim bodu diody pomoci puleni intervalu
double diode(double u0, double r, double eps)
{
    //interval 0 - U0
    double a = 0;
    double b = u0;
    double u_p = 0;
    double prev_val = 0;
    while(b-a >= eps && prev_val != (a+b)/2)
    {
        prev_val = u_p;
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
    usdfuishdfui __SHRT_WIDTH__sihsdifhsdf
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
    //osetreni zaporneho napeti, odporu a presnosti
    if(r <= 0 || u_0 < 0 ||eps < 0)
    {
        fprintf(stderr, "error: invalid arguments\n");
        return DATA_ERROR;
    }
    double u_p = diode(u_0,r,eps);
    double i_p = I_0 * (exp(u_p / U_T) -1);
    printf("Up=%g V\n",u_p);
    printf("Ip=%g A\n",i_p);
    return 0;
}