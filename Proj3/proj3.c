#include <stdio.h>
#include <string.h>
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

void print_help()
{
    printf("--test soubor \n");
    printf("    otestuje zadany soubor, jestli obsahuje validni definici mapy \n");
    printf("--rpath  R C soubor.txt \n");
    printf("    Najde cestu z bludiste zadaneho v souboru 'soubor.txt' z radku R a sloupce C pomoci metody prave ruky\n");
    printf("--lpath  R C soubor.txt \n");
    printf("    Najde cestu z bludiste zadaneho v souboru 'soubor.txt' z radku R a sloupce C pomoci metody leve ruky\n");
}
int main(int argc, char **argv)
{
    if(argc == 2 && strcmp(argv[1], "--help") == 0)
    {
            print_help();
            return 0;
    }
    
    
    return 0;
}