#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 100

//pismena v retezci prevede na mala pismena
void to_lower(char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
    }
}

//overeni, zda jsou ve vstupu pouze cisla
bool input_valid(char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}

//Odstraneni znaku \n, prip. \r, aby bylo mozne vypisovat na jeden radek
void remove_newline(char *str)
{
    int length = strlen(str);
    if (str[length - 1] == '\n')
    {
        str[length - 1] = '\0';
    }
    if (str[length - 2] == '\r')
    {
        str[length - 2] = '\0';
    }
}

//globalni pole zastupujicich znaku
char *key_chars[] = {"+", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

//cislo kontaktu se shoduje s filtrem
bool number_matches_filter(char *contact_number, char *input_filter)
{
    int current_filter_pos = 0;
    int input_length = strlen(input_filter);
    int number_length = strlen(contact_number);
    for (int i = 0; i < number_length; i++)
    {
        //znak je primo cislo, prip kdyz je 0 muze byt i +
        if (contact_number[i] == input_filter[current_filter_pos] || (input_filter[current_filter_pos] == '0' && contact_number[i] == '+'))
        {
            current_filter_pos++;
            //je zkontrolovan cely vstup
            if (current_filter_pos == input_length)
            {   
                return true;
            }
        }
    }
    return false;
}

//jmeno kontaktu se shoduje s filtrem
bool name_matches_filter(char *contact_name, char *input_filter)
{
    int current_filter_pos = 0;
    int input_length = strlen(input_filter);
    int name_length = strlen(contact_name);
    for (int i = 0; i < name_length; i++)
    {
        //prevod aktualniho kontrolovaneho cislo z char do int
        int key_number = (int)(input_filter[current_filter_pos] - '0');
        //pokud je znak přímo číslo, jinak zastupující znaky
        if (contact_name[i] == input_filter[current_filter_pos])
        {
            current_filter_pos++;
        }
        else
        {
            int key_chars_length = strlen(key_chars[key_number]);
            //zkontrolovani kazdeho zastupujiciho znaku
            for (int j = 0; j < key_chars_length; j++)
            {
                if (contact_name[i] == key_chars[key_number][j])
                {
                    current_filter_pos++;
                    break;
                }
            }
        }
        //je zkontrolovano cele jmeno
        if (input_length == current_filter_pos)
        {
            return true;
        }
    }
    return false;
}

//vypsani vsech kontaktu v seznamu
void write_all_contacts()
{
    char contact_name[MAX_CHARS+2];
    char contact_number[MAX_CHARS+2];
        while (fgets(contact_name, MAX_CHARS + 1, stdin) && fgets(contact_number, MAX_CHARS + 1, stdin))
        {
            remove_newline(contact_name);
            remove_newline(contact_number);
            printf("%s, %s \n", contact_name, contact_number);
        }
}
int main(int argc, char *argv[])
{
    //+2 - do delky se nepocitaji znaky /n a /0
    char contact_name[MAX_CHARS + 2];
    char contact_number[MAX_CHARS + 2];
    int found_matches = 0;
    //Neni zadan filtr - vypis kontaktu, konec programu
    if (argc < 2)
    {
        write_all_contacts();
        return 0;
    }
    char* filter = argv[1];
    //Vstupni filtr obsahuje neco jineho nez cislice
    if (!input_valid(filter))
    {
        fprintf(stderr, "Vstup musí být pouze čísla \n");
        return 1;
    }
    //cteni jmena a cisla
    while (fgets(contact_name, MAX_CHARS + 2, stdin) && fgets(contact_number, MAX_CHARS + 2, stdin))
    {
        to_lower(contact_name); //nezalezi na velikosti pismen
        //vypis kontaktu odpovidajici filtru
        if (number_matches_filter(contact_number, filter) || name_matches_filter(contact_name, filter))
        {
            remove_newline(contact_name);
            remove_newline(contact_number);
            printf("%s, %s \n", contact_name, contact_number);
            found_matches++;   
        }
    }
    if (found_matches == 0)
    {
        printf("Not found\n");
    }
    return 0;
}