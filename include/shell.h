#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT 1024
#define MAX_ARGS 100

void print_prompt();
void execute_command(char *command);
void handle_redirection(char **args);
int handle_pipe(char *command);
void handle_increment(char **args);
void execute_sequence(char *input);

#endif // SHELL_H
