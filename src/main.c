#include <stdio.h>
#include <string.h>
#include "shell.h"

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