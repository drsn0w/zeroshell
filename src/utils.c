#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char* utils_getline() {
    int buffer_size = 512;
    int position = 0;
    char* buffer = malloc(sizeof(char) * buffer_size);
    if(!buffer) {
        fprintf(stderr, "zeroshell: unable to allocate input buffer\n");
        return NULL;
    }

    char c;
    while(1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= buffer_size) {
            buffer_size += 512;
            buffer = realloc(buffer, buffer_size);
            if(!buffer) {
                fprintf(stderr, "zeroshell: unable to extend input buffer\n");
                return NULL;
            }
        }
    }
}

char** utils_tokenize(char* input, char* delimiter) {
    int buffer_size = 512;
    int position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    if(!tokens) {
        fprintf(stderr, "zeroshell: unable to allocate input buffer\n");
        return NULL;
    }
    char* token;
    token = strtok(input, delimiter);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if(position >= buffer_size) {
            buffer_size += 512;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "zeroshell: unable to reallocate input buffer\n");
                return NULL;
            }
        }
        token = strtok(NULL, delimiter);

    }
    tokens[position] = NULL;

    return tokens;
}
