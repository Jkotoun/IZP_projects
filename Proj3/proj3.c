#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//kody chyb
enum ERRORS
{
    ARGS_ERROR = 1,
    FILE_ERROR,
    MAP_LOADING_ERROR
};

//kody hranic policek
enum BORDERS
{
    BORDER_LEFT = 1,
    BORDER_RIGHT,
    BORDER_TOP_BOT = 4
};
//metody vyhledavani
enum PATHFINDING_METHOD
{
    RIGHT_HAND,
    LEFT_HAND
};
//struktura pro ulozeni moznych pohybu v bludisti
typedef struct triangle_move
{
    int x;
    int y;
} Triangle;
//struktura pro mapu bludiste
typedef struct map
{
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
    for (int i = 0; i < (map->cols * map->rows); i++)
    {
        map->cells[i] = 0;
    }
}
//je na dane hranici stena? - rozhoduje podle jednotlivych bitu cisel ze souboru
bool isborder(Map *map, int r, int c, int border)
{
    char cell = map->cells[r * map->cols + c];
    int cell_val = cell - '0'; //cislice z ordinalni hodnoty znaku
    return (border & cell_val) == border;
}
//nacitani mapy ze souboru do datove struktury map
int map_load(Map *map, FILE *f)
{
    unsigned char cell_info;
    int row_pos = 0;
    int col_pos = 0;
    int success;
    while ((success = (fscanf(f, "%cu", &cell_info))) != EOF)
    {
        if (success == 0)
        {
            return MAP_LOADING_ERROR;
        }
        if (cell_info != ' ' && cell_info != '\n' && cell_info != '\r')
        {
            if (row_pos >= map->rows || col_pos >= map->cols || !isdigit(cell_info))
            {
                return MAP_LOADING_ERROR;
            }
            map->cells[map->cols * row_pos + col_pos] = cell_info;
            col_pos++;
            if (col_pos > map->cols - 1)
            {
                col_pos = 0;
                row_pos++;
            }
        }
    }
    return 0;
}
//vrati prvni referencni hranici podle zpusobu hledani
int start_border(Map *map, int r, int c, int leftright)
{
    //odecteni 1 - indexy od 0
    c = c - 1;
    r = r - 1;
    if (c == 0 || c == map->cols - 1 || r == 0 || r == map->rows - 1)
    {
        //prvni radek
        if (r == 0)
        {
            //prvni sloupec
            if (c == 0 && !isborder(map, r, c, BORDER_LEFT))
            {
                return leftright ? BORDER_TOP_BOT : BORDER_RIGHT;
            }
            //posledni sloupec
            else if (c == map->cols - 1 && !isborder(map, r, c, BORDER_RIGHT))
            {
                return leftright ? BORDER_LEFT : BORDER_TOP_BOT;
            }
            else
            {
                return leftright ? BORDER_RIGHT : BORDER_LEFT;
            }
        }
        //posledni radek
        else if (r == map->rows - 1)
        {
            //prvni sloupec
            if (c == 0 && !isborder(map, r, c, BORDER_LEFT))
            {
                if (r % 2 == 0)
                    return leftright ? BORDER_TOP_BOT : BORDER_RIGHT;
                else
                    return leftright ? BORDER_RIGHT : BORDER_TOP_BOT;
            }
            //posledni sloupec
            else if (c == map->cols - 1 && !isborder(map, r, c, BORDER_RIGHT))
            {
                if (r % 2 == 0)
                    return leftright ? BORDER_LEFT : BORDER_TOP_BOT;
                else
                    return leftright ? BORDER_TOP_BOT : BORDER_LEFT;
            }
            else
            {
                return leftright ? BORDER_LEFT : BORDER_RIGHT;
            }
        }
        //ostatni sude
        else if (r % 2 == 0)
        {
            if (c == 0)
            {
                return leftright ? BORDER_TOP_BOT : BORDER_RIGHT;
            }

            else if (c == map->cols - 1)
            {
                return leftright ? BORDER_LEFT : BORDER_TOP_BOT;
            }
        }
        //ostatni liche
        else
        {
            if (c == 0)
            {
                return leftright ? BORDER_RIGHT : BORDER_TOP_BOT;
            }
            else if (c == map->cols - 1)
            {
                return leftright ? BORDER_TOP_BOT : BORDER_LEFT;
            }
        }
    }
    return -1;
}
//funkce pro najiti cesty ven podle zadanych policek na kraji, metody hledani a prvni referencni hranice
void pathfinding(Map *map, int r, int c, int start_border, int finding_method)
{
    //normalni trojuhelnik
    Triangle tr1_directions[] =
        {
            {0, 1}, //dolu
            {1, 0}, //doprava
            {-1, 0} //doleva
        };
    //otoceny trojuhelnik
    Triangle tr2_directions[] =
        {
            {1, 0},  //doprava
            {0, -1}, //nahoru
            {-1, 0}  //doleva
        };
    //normalni - serazene smery podle pole smeru
    int tr1_borders[] = {BORDER_TOP_BOT, BORDER_RIGHT, BORDER_LEFT};
    //otoceny
    int tr2_borders[] = {BORDER_RIGHT, BORDER_TOP_BOT, BORDER_LEFT};
    int current_row = r - 1;
    int current_col = c - 1;
    int last_move;
    //najiti predchoziho smeru pro prvni krok podle odpovidajici startovni hranice
    //hodnoty indexu podle vztahu indexu predchoziho smeru a odpovidajiciho prioritniho smeru v druhem trojuhelniku
    //hranice maji stejny index v poli enumu jako odpovidajici smery
    //normalni
    if (current_row % 2 != current_col % 2)
    {
        int i = 0;
        if (finding_method == LEFT_HAND)
        {
            while (tr1_borders[(i + 1) % 3] != start_border)
                i++;
        }
        else
        {
            while (tr1_borders[i] != start_border)
                i++;
        }
        last_move = i;
    }
    //otoceny
    else
    {
        int i = 0;
        if (finding_method == LEFT_HAND)
        {
            while (tr2_borders[i] != start_border)
                i++;
        }
        else
        {
            while (tr2_borders[(i + 2) % 3] != start_border)
                i++;
        }
        last_move = i;
    }
    int next_move;
    int next_x, next_y;
    while (current_row < map->rows && current_row >= 0 && current_col < map->cols && current_col >= 0)
    {
        //normalni
        if (current_row % 2 != current_col % 2)
        {
             //index prioritiniho kroku v normalnim trojuhelniku =  index predchoziho kroku + 1
            if (finding_method == LEFT_HAND)
            {
                next_move = (last_move + 1) % 3;
            }
            //index prioritiniho kroku v normalnim trojuhelniku = index predchoziho kroku
            else
            {
                next_move = last_move;
            }
            while (isborder(map, current_row, current_col, tr1_borders[next_move])) //zmena smeru pokud je na danem smeru stena - L_hand = -1, R_hand = +1
            {
                if (finding_method == LEFT_HAND)
                {
                    next_move = (next_move - 1 + 3) % 3;
                }
                else
                {
                    next_move = (next_move + 1) % 3;
                }
            }
            next_x = tr1_directions[next_move].x;
            next_y = tr1_directions[next_move].y;
        }
        //otoceny
        else
        {
            //index prioritiniho kroku v otocenem trojuhelniku = index predchoziho kroku
            if (finding_method == LEFT_HAND)
            {
                next_move = last_move;
            }
            //index prioritiniho kroku v otocenem trojuhelniku = index predchoziho kroku + 2
            else
            {
                next_move = (last_move + 2) % 3;
            }
            while (isborder(map, current_row, current_col, tr2_borders[next_move])) //zmena smeru pokud je na danem smeru stena - L_hand = -1, R_hand = +1
            {
                if (finding_method == LEFT_HAND)
                {
                    next_move = (next_move - 1 + 3) % 3;
                }
                else
                {
                    next_move = (next_move + 1) % 3;
                }
            }
            next_x = tr2_directions[next_move].x;
            next_y = tr2_directions[next_move].y;
        }
        printf("%d,%d\n", current_row + 1, current_col + 1);
        current_row += next_y;
        current_col += next_x;
        last_move = next_move;
    }
}

//overeni validity mapy, vraci logickou hodnotu
bool isvalid_map(Map *map)
{
    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            //nevalidni popis - jina hodnota nez 0 - 7, pripadne neni popis vubec - ordinalni hodnota 0 z inicializace mapy
            if (map->cells[i * map->cols + j] == 0 || (map->cells[i * map->cols + j] < '0' || map->cells[i * map->cols + j] > '7'))
                return false;
            //prava strana ma shodnou hranici s vedlejsim trojuhelnikem
            if (j + 1 != map->cols)
            {
                if (isborder(map, i, j, BORDER_RIGHT) != isborder(map, i, j + 1, BORDER_LEFT))
                {
                    return false;
                }
            }
            //leva strana ma shodnou hranici s vedlejsim trojuhelnikem
            if (j != 0)
            {
                if (isborder(map, i, j, BORDER_LEFT) != isborder(map, i, j - 1, BORDER_RIGHT))
                    return false;
            }
            //horni hranice - sudy sloupec a sudy radek nebo lichy sloupec a lichy radek
            if (i % 2 == j % 2)
            {
                if (i != 0)
                {
                    if (isborder(map, i, j, BORDER_TOP_BOT) != isborder(map, i - 1, j, BORDER_TOP_BOT))
                        return false;
                }
            }
            //sudy radek lichy sloupec nebo lichy sloupec sudy radek - maji spodni hranici
            else
            {
                if (i + 1 != map->rows)
                {
                    if (isborder(map, i, j, BORDER_TOP_BOT) != isborder(map, i + 1, j, BORDER_TOP_BOT))
                        return false;
                }
            }
        }
    }
    return true;
}
//je retezec z cislic
bool str_is_num(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}
void print_help()
{
    printf("--test soubor.txt \n");
    printf("    otestuje zadany soubor, jestli obsahuje validni definici mapy \n");
    printf("--rpath  R C soubor.txt \n");
    printf("    Najde cestu z bludiste zadaneho v souboru 'soubor.txt' z radku R a sloupce C pomoci metody prave ruky\n");
    printf("--lpath  R C soubor.txt \n");
    printf("    Najde cestu z bludiste zadaneho v souboru 'soubor.txt' z radku R a sloupce C pomoci metody leve ruky\n");
}
int main(int argc, char **argv)
{
    //argument je napoveda
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        print_help();
        return 0;
    }
    else if (argc > 2)
    {
        bool test_only = false;
        char *file_name;
        //program je spusten jako test validity bludiste ze souboru
        if (strcmp(argv[1], "--test") == 0)
        {
            test_only = true;
            file_name = argv[2];
        }
        //program je spusten pro vyhledani cesty v bludisti s validnimi argumenty
        else if (argc == 5 && str_is_num(argv[2]) && str_is_num(argv[3]) && (strcmp(argv[1], "--lpath") == 0 || strcmp(argv[1], "--rpath") == 0))
        {
            file_name = argv[4];
        }
        else
        {
            fprintf(stderr, "Invalid arguments\n");
            return ARGS_ERROR;
        }
        //otevreni souboru s bludistem, overeni jestli existuje
        FILE *maze_file = fopen(file_name, "r");
        if (maze_file == NULL)
        {
            fprintf(stderr, "Soubor nenalezen\n");
            return FILE_ERROR;
        }
        //nacteni poctu radku a sloupcu ze souboru (prvni a druhe cislo na prvnim radku)
        int maze_rows, maze_cols;
        int loaded_nums = fscanf(maze_file, "%d %d [\r][\n]", &maze_rows, &maze_cols);
        if (loaded_nums == 0)
        {
            fprintf(stderr, "Chyba cteni ze souboru\n");
            return FILE_ERROR;
        }
        //alokovani pameti pro mapu a inicializace na pocatecni hodnoty
        Map maze_map = map_ctor(maze_rows, maze_cols);
        map_init(&maze_map);
        int load_result = map_load(&maze_map, maze_file);
        if (load_result == MAP_LOADING_ERROR)
        {
            fprintf(stderr, "Invalid\n");
            fclose(maze_file);
            return MAP_LOADING_ERROR;
        }
        else
        {
            fclose(maze_file);
        }
        //test validity souboru
        if (test_only)
        {
            isvalid_map(&maze_map) ? printf("Valid\n") : printf("Invalid\n");
            return 0;
        }
        //vyhledani cesty v bludisti
        else
        {
            if (isvalid_map(&maze_map))
            {
                int start_r = atoi(argv[2]);
                int start_c = atoi(argv[3]);
                if (start_r > maze_map.rows || start_c > maze_map.cols)
                {
                    fprintf(stderr, "Zadane hodnoty jsou mimo rozsah mapy bludiste\n");
                    return ARGS_ERROR;
                }
                if (strcmp(argv[1], "--lpath") == 0)
                {
                    int border_st = start_border(&maze_map, start_r, start_c, LEFT_HAND);
                    if (border_st == -1)
                    {
                        fprintf(stderr, "Nespravna pocatecni hranice\n");
                        return ARGS_ERROR;
                    }
                    pathfinding(&maze_map, start_r, start_c, border_st, LEFT_HAND);
                }
                else
                {
                    int border_st = start_border(&maze_map, start_r, start_c, RIGHT_HAND);
                    if (border_st == -1)
                    {
                        fprintf(stderr, "Nespravna pocatecni hranice");
                        return ARGS_ERROR;
                    }
                    pathfinding(&maze_map, start_r, start_c, border_st, RIGHT_HAND);
                }
            }
            else
            {
                fprintf(stderr, "Invalid");
                return MAP_LOADING_ERROR;
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