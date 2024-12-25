#include <stdio.h>
#include <string.h>
#include "shell.h"

//GRUP ADI: 12AB Grubu
//GRUP UYELERI
//Ömer Faruk Aydın - G221210370 - 1.Öğretim A Grubu
//Tarık Kartal - Y245012016 - 1. Öğretim A Grubu
//Harun Mahmut Yeşilyurt - G211210089 - 2. Öğretim B Grubu
//Batuhan Bahayetmez - G211210043 - 2. Öğretim A Grubu
//Talha İmran Göneç - G221210032 - 2. Öğretim B Grubu

int main()
{
    char input[MAX_INPUT];

    while (1)
    {
        print_prompt();
        if (fgets(input, MAX_INPUT, stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }

        execute_sequence(input);
    }

    return 0;
}
