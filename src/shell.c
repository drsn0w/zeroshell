#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "utils.h"

void zeroshell_loop(char** env) {
    bool should_exit = false;
    char* input;
    while(!should_exit) {
        printf("zerosh$ ");
        input = utils_getline();
        if(!strcmp(input, "exit")) {
            should_exit = true;
        } else if (!strcmp(input, "printenv")) {
            int pos = 0;
            while(env[pos] != NULL) {
                printf("%s\n", env[pos]);
                pos++;
            }
        } else if(input[0] == '\0' || input[0] == ' ') {
            // If they enter nothing, do nothing
        }
        else {
            zeroshell_launch(input, env);
        }
        free(input);
    }
}

void zeroshell_launch(char* input, char** env) {
    char** argv = utils_tokenize(input, " ");
    // Search path for program, error if not found
    // Copy contents of path_env into path - if we strtok through the *path_env it will actually modify PATH inside the process' environment, which we do not want to do
    char* path_env = getenv("PATH");
    char* path = malloc(strlen(path_env));
    strcpy(path, path_env);
    char** path_tok = utils_tokenize(path, ":");
    int pos = 0;
    char* concatenated;
    while (path_tok[pos] != NULL) {
        // concatenate path and name of program
        size_t concatenated_size = strlen(path_tok[pos]) + strlen(argv[0]) + 1;
        concatenated = malloc(concatenated_size);
        snprintf(concatenated, concatenated_size+1, "%s/%s", path_tok[pos], argv[0]);
        if(!access(concatenated, X_OK)) break;
        pos++;
    }
    free(path);
    pid_t pid = fork();
    int status;

    switch(pid) {
        case -1:
            perror("zeroshell");
            exit(EXIT_FAILURE);
        case 0:
            // Child
            execve(concatenated, argv, env);
            // don't expect to return
            perror("zeroshell");
            exit(EXIT_FAILURE);
        default:
            waitpid(pid, &status, WUNTRACED);
    }
    free(argv);
    free(path_tok);
    free(concatenated);
}
