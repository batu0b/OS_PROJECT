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

//  Girilen komutu çalıştırır (örn: "ls", "cat", vb.)
void execute_command(char *command)
{
    // Harun Yesilyurt G211210089
    char *args[MAX_ARGS];
    int i = 0;
    int background = 0;

    // Komutları ayrıştır
    args[i] = strtok(command, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1)
    {
        if (strcmp(args[i], "&") == 0)
        {
            background = 1;
            args[i] = NULL;
            break;
        }
        i++;
        args[i] = strtok(NULL, " ");
    }

    // quit komutu
    if (args[0] != NULL && strcmp(args[0], "quit") == 0)
    {
        exit(0);
    }

    // increment komutu
    else if (args[0] != NULL && strcmp(args[0], "increment") == 0)
    {
        handle_increment(args);
        return;
    }

    // Alt süreç oluştur ve çalıştır
    pid_t pid = fork();
    if (pid == 0)
    {
        // Çocuk süreç
        handle_redirection(args);
        if (execvp(args[0], args) == -1)
        {
            perror("Komut çalıştırılamadı");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Ana süreç
        if (!background)
        {
            int status;
            waitpid(pid, &status, 0);
        }
        else
        {
            printf("[PID: %d] Arka planda çalışıyor\n", pid);
        }
    }
    else
    {
        perror("Fork başarısız oldu");
    }
}

//  Komut içindeki dosya yönlendirmesini işler (">", "<" gibi)
void handle_redirection(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            // Giriş yönlendirme
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0)
            {
                perror("Giriş dosyası açılamadı");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            // Çıkış yönlendirme
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Çıkış dosyası açılamadı");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}

//  Pipe'lı (|) komutları işler
int handle_pipe(char *command)
{
    char *commands[MAX_ARGS];
    int i = 0;

    // Komutları pipe ile ayır
    commands[i] = strtok(command, "|");
    while (commands[i] != NULL && i < MAX_ARGS - 1)
    {
        i++;
        commands[i] = strtok(NULL, "|");
    }

    int num_commands = i;
    int pipefds[2 * (num_commands - 1)];

    // Tüm pipe'ları oluştur
    for (int j = 0; j < num_commands - 1; j++)
    {
        if (pipe(pipefds + 2 * j) < 0)
        {
            perror("Pipe oluşturulamadı");
            exit(EXIT_FAILURE);
        }
    }

    for (int j = 0; j < num_commands; j++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // İlk komut
            if (j != 0)
            {
                dup2(pipefds[2 * (j - 1)], STDIN_FILENO);
            }
            // Son komut değilse
            if (j != num_commands - 1)
            {
                dup2(pipefds[2 * j + 1], STDOUT_FILENO);
            }

            // Tüm pipe'ları kapat
            for (int k = 0; k < 2 * (num_commands - 1); k++)
            {
                close(pipefds[k]);
            }

            char *args[MAX_ARGS];
            int k = 0;
            args[k] = strtok(commands[j], " ");
            while (args[k] != NULL && k < MAX_ARGS - 1)
            {
                k++;
                args[k] = strtok(NULL, " ");
            }

            // Increment iç komutunu ele al
            if (strcmp(args[0], "increment") == 0)
            {
                handle_increment(args);
                exit(EXIT_SUCCESS);
            }

            execvp(args[0], args);
            perror("Komut çalıştırılamadı");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("Fork başarısız oldu");
            exit(EXIT_FAILURE);
        }
    }

    // Tüm pipe'ları kapat
    for (int j = 0; j < 2 * (num_commands - 1); j++)
    {
        close(pipefds[j]);
    }

    // Tüm çocuk süreçlerin bitmesini bekle
    for (int j = 0; j < num_commands; j++)
    {
        wait(NULL);
    }

    return 0;
}

// TODO
//  "increment" komutunu işler ve bir sayıyı artırır
void handle_increment(char **args) { /* ... */ }
