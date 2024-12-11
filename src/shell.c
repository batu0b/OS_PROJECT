#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

// Kullanıcıya "SAU >" şeklinde bir komut istemi yazdırır
void print_prompt()
{
    // Batuhan Bahayetmez G211210043
    printf("SAU > ");
    fflush(stdout);
}

// Kullanıcının girdiği birden fazla komutu, ";" ile ayırarak sıralı bir şekilde çalıştırır
void execute_sequence(char *input)
{
    // Batuhan Bahayetmez G211210043
    char *commands[MAX_ARGS];
    int i = 0;

    // Komutları noktalı virgül ile ayır
    commands[i] = strtok(input, ";");
    while (commands[i] != NULL && i < MAX_ARGS - 1)
    {
        i++;
        commands[i] = strtok(NULL, ";");
    }

    for (int j = 0; j < i; j++)
    {
        // Komutların başındaki ve sonundaki boşlukları temizle
        char *command = commands[j];
        while (*command == ' ')
            command++;
        char *end = command + strlen(command) - 1;
        while (end > command && *end == ' ')
            *end-- = '\0';

        if (strlen(command) == 0)
            continue;

        // Pipe içeren komutları ele al
        if (strchr(command, '|'))
        {
            handle_pipe(command);
        }
        else
        {
            execute_command(command);
        }
    }
}

// TODO
//  Girilen komutu çalıştırır (örn: "ls", "cat", vb.)
void execute_command(char *command) { /* ... */ }

// TODO
//  Komut içindeki dosya yönlendirmesini işler (">", "<" gibi)
void handle_redirection(char **args) { /* ... */ }

// TODO
//  Pipe'lı (|) komutları işler
int handle_pipe(char *command) { /* ... */ }

// TODO
//  "increment" komutunu işler ve bir sayıyı artırır
void handle_increment(char **args) { /* ... */ }
