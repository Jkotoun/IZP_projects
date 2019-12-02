#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//struktura pro mapu bludiste
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;


//konstruktor mapy - alokace pameti, nastaveni hodnoty radku a sloupcu
Map map_ctor(int rows, int cols)
{
    Map map;
    map.cols = cols;
    map.rows = rows;
    map.cells = malloc(sizeof(unsigned char) * rows * cols);
    return map;
}

//uvolneni pameti po mape
void map_dtor(Map *map)
{
    free(map->cells);
    map->rows = 0;
    map->cols = 0;
}

//inicializace vychozich hodnot na alokovanou pamet
void map_init(Map *map)
{
    for(int i=0;i<(map->cols * map->rows);i++)
    {
        map->cells[i] = 0;
    }
}
//kody chyb
enum ERRORS {ARGS_ERROR = 1, FILE_ERROR};
//Funkce vracejici logickou hodnotu podle toho, jestli je retezec cislo
bool str_is_num(char *str)
{
    for(int i=0;str[i]!='\0';i++)
    {
        if(!isdigit(str[i]))
            return false;
    }
    return true;
}
//vypise napovedu
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
    //pokud je argument napoveda
    if(argc == 2 && strcmp(argv[1], "--help") == 0)
    {
            print_help();
            return 0;
    }
    //pokud je argument test se zadanym souborem
    else if(argc == 3 && strcmp(argv[1], "--test") == 0)
    {
        FILE *f = fopen(argv[2],"r");
        if(f == NULL)
        {
            fprintf(stderr, "Soubor nenalezen\n");
            return FILE_ERROR;
        }
    }
    //pokud jsou zadany argumenty pro vyhledavani v bludisti (metoda vyhledavani, pocatecni radek a sloupec a txt soubor)
    else if(argc == 5 && str_is_num(argv[2]) && str_is_num(argv[3]) && (strcmp(argv[1], "--lpath") == 0 || strcmp(argv[1], "--rpath") == 0))
    {
        //otevreni souboru
        FILE *maze_file = fopen(argv[4], "r");
        if(maze_file == NULL)
        {
            fprintf(stderr, "Soubor nenalezen");
            return FILE_ERROR;
        }
        //nacteni poctu radku a sloupcu ze souboru (2 cisla na prvnim radku)
        int maze_rows, maze_cols;
        fscanf(maze_file,"%d %d", &maze_rows, &maze_cols);
        //alokovani pameti pro mapu a inicializace na pocetaceni hodnoty
        Map maze_map = map_ctor(maze_rows,maze_cols);
        map_init(&maze_map);
        if(strcmp(argv[1], "--lpath") == 0)
        {
            //LPATH
            printf("lpath\n");
        }
        else
        {
            //RPATH
            printf("rpath\n");
        }
        //uvolneni pameti po mapě
        map_dtor(&maze_map);
    }
    else
    {
        fprintf(stderr, "Invalid arguments\n");
        return ARGS_ERROR;
    }
    return 0;
}