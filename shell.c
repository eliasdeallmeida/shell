#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TRUE 1
#define MAX_CMD_LEN 100
#define CWD_LEN 100

void type_prompt();
void read_command(char*);
void execute_command(char*);

int main() {
    char command[MAX_CMD_LEN];

    while (TRUE) {
        type_prompt();
        read_command(command);
        
        if (strcmp(command, "exit") == 0) {
            break;
        }

        execute_command(command);
    }

    return 0;
}

void type_prompt() {
    char cwd[CWD_LEN];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\033[0;32m%s\033[0m$ ", cwd);
    }
    else {
        perror("getcwd() error");
    }
}

void read_command(char *command) {
    if (fgets(command, MAX_CMD_LEN, stdin) != NULL) {  
        command[strcspn(command, "\n")] = '\0';
    }
}

void execute_command(char *command) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar processo filho");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        char *args[MAX_CMD_LEN];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;
        
        if (execvp(args[0], args) == -1) {
            perror("Erro ao executar comando");
            exit(EXIT_FAILURE);
        }
    }
    else {
        int status;
        waitpid(pid, &status, 0);
    }
}
