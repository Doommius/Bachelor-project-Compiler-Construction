#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

void print_error(char *error, int code, int line){
    fprintf(stderr, "%s at line %d\n", error, line);
    exit(code);
}