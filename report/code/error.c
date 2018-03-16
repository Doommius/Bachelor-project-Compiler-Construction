/**
 * @brief Error printing.
 * 
 * @file error.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

void print_error(char *error, int code, int line){
    fprintf(stderr, "%s at line %d\n", error, line);
    exit(code);
}