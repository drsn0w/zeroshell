#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

int main(int argc, char** argv, char** env) {
    zeroshell_loop(env);
    return 0;
}