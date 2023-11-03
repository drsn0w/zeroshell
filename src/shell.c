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
        } else if(input[0] == ((void*)0)) {
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
        concatenated = malloc(strlen(path_tok[pos]) + strlen(argv[0]) + 1);
        strcpy(concatenated, path_tok[pos]);
        strcat(concatenated, "/");
        strcat(concatenated, argv[0]);
        int access_result = access(concatenated, X_OK);
        if(access_result == 0) {
            printf("%s is valid\n", concatenated);
            break;
        }
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
            printf("executing %s\n", argv[0]);
            execvp(concatenated, argv);
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