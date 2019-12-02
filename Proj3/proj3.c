#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//struktura pro mapu bludiste
typedef struct map{
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
enum ERRORS {ARGS_ERROR = 1, FILE_ERROR, FILE_ACCESS_ERROR};
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
    else if(argc > 2)
    {
        bool test_only = false;
        char *file_name;
        //program je  spusten pouze jako test validity bludiste v souboru
        if(strcmp(argv[1], "--test") == 0)
        {
            test_only = true;
            file_name = argv[2];
        }
        //program je spusten pro vyhledani cesty v bludisti s validnimi argumenty 
        else if(argc == 5 && str_is_num(argv[2]) && str_is_num(argv[3]) && (strcmp(argv[1], "--lpath") == 0 || strcmp(argv[1], "--rpath") == 0))
        {
            file_name = argv[4];
        }
        else
        {
            fprintf(stderr, "Invalid arguments\n");
            return ARGS_ERROR;
        }
        //otevreni souboru s bludistem, overeni jestli existuje
        FILE *maze_file = fopen(file_name,"r");
        if(maze_file == NULL)
        {
            fprintf(stderr, "Soubor nenalezen\n");
            return FILE_ERROR;
        }
        //nacteni poctu radku a sloupcu ze souboru (prvni a druhe cislo na prvnim radku)
        int maze_rows, maze_cols;
        int loaded_nums = fscanf(maze_file,"%d %d", &maze_rows, &maze_cols);
        if(loaded_nums == 0)
        {
            fprintf(stderr, "Chyba cteni ze souboru");
            return FILE_ACCESS_ERROR;
        }
        //alokovani pameti pro mapu a inicializace na pocatecni hodnoty
        Map maze_map = map_ctor(maze_rows,maze_cols);
        map_init(&maze_map);
        //test validity souboru
        if(test_only)
        {
            //test()
            return 0;
        }
        //vyhledani cesty v bludisti
        else
        {
            if(strcmp(argv[1], "--lpath") == 0)
            {
                //TODO LPATH()
            }
            else
            {
                //TODO RPATH()
            }
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