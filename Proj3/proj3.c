#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>



//kody chyb
enum ERRORS {ARGS_ERROR = 1, FILE_ERROR, MAP_LOADING_ERROR};

//kody hranic policek
enum BORDERS {LEFT = 1, RIGHT, UP_DOWN = 4};
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

int map_load(Map *map, FILE *f)
{
    char cell_info;
    int row_pos = 0;
    int col_pos = 0;
    int success;
    while((success = (fscanf(f, "%c", &cell_info))) != EOF)
    {
        if(success==0)
        {
            return MAP_LOADING_ERROR;
        }
        if (cell_info != ' ' && cell_info != '\n' &&cell_info != '\r')
        {
            if(row_pos >= map->rows || col_pos >= map->cols)
            {
                return MAP_LOADING_ERROR;
            }
            if(!isdigit(cell_info))
                return MAP_LOADING_ERROR;
            map->cells[map->cols * row_pos + col_pos] = cell_info;
            col_pos++;
            if(col_pos > map->cols-1)
            {
                col_pos = 0;
                row_pos++;
            }
        }
    }
    return 0;
}
//je na dane hranici stena? - rozhoduje podle jednotlivych bitu cisel ze souboru, jako hodnoty border predpoklada binarni hodnoty dane hranice (1|2|4)
bool isborder(Map *map, int r, int c, int border)
{
    char cell = map ->cells[r * map->cols + c];
    int cell_val = cell - '0';
    return (border & cell_val) == border; 
}

bool isvalid_map(Map *map)
{
    for(int i=0;i<map->rows;i++)
    {
        for(int j=0;j<map->cols;j++)
        {
            //je popis pro dane policko
            if(map->cells[i * map->cols + j] == 0)
                return false;
            //prava strana ma shodnou hranici s vedlejsim trojuhelnikem
            if(j+1 !=map->cols)
            {
                if(isborder(map,i,j,RIGHT) != isborder(map,i,j+1,LEFT))
                {
                    return false;   
                }
                    
            }
            //leva strana ma shodnou hranici s vedlejsim trojuhelnikem
            if(j!=0)
            {
                if(isborder(map,i,j,LEFT) != isborder(map,i,j-1,RIGHT))
                    return false;
            }
                 //shoda spodni / horni hranice, sude a liche podle indexu od 0
                //horni hranice - sudy sloupec a sudy radek nebo lichy sloupec a lichy radek
                if(i % 2 == j % 2)
                {
                    if(i!=0)
                    {
                        if(isborder(map,i,j,UP_DOWN) != isborder(map,i-1,j,UP_DOWN))
                            return false;
                    }
                }
                //sudy radek lichy sloupec nebo lichy sloupec sudy radek - maji spodni hranici
                else
                {
                    if(i+1!= map->rows)
                    {
                        if(isborder(map,i,j,UP_DOWN) != isborder(map,i+1,j,UP_DOWN))
                        return false;
                    }
                 }
        }

    }
    return true;
}

//mazeprint - test function 
void maze_print(Map *map)
{
    int col_pos = 0;
    int row_pos  = 0;

    for(int i=0;i<map->cols * map->rows;i++)
    {
        if(col_pos == map->cols)
        {
            printf("\n");
            col_pos = 0;
            row_pos++;
        }
        printf(" %c ", map->cells[row_pos * map->cols + col_pos]);
        col_pos++;
            
    }    
    printf("\n");
}

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
        int loaded_nums = fscanf(maze_file,"%d %d [\r][\n]", &maze_rows, &maze_cols);
        if(loaded_nums == 0)
        {
            fprintf(stderr, "Chyba cteni ze souboru");
            return FILE_ERROR;
        }
        //alokovani pameti pro mapu a inicializace na pocatecni hodnoty
        Map maze_map = map_ctor(maze_rows,maze_cols);
        map_init(&maze_map);

        int load_result = map_load(&maze_map,maze_file);
        if(load_result == MAP_LOADING_ERROR)
        {
            fprintf(stderr, "Nepodarilo se nacist mapu");
            return MAP_LOADING_ERROR;
        }
        //test validity souboru
        if(test_only)
        {
            bool valid = isvalid_map(&maze_map);
            if(valid)
            {
                printf("Valid\n");
            }
            else
            {
                printf("Invalid\n");
            }
            
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