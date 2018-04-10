/**
 * @brief Compiler program.
 * 
 * @file main.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include "debug.h"

#include "main.h"
#include "auxiliary.h"
#include "symbol.h"
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"
#include "weeder.h"
#include "typechecker.h"
#include "code.h"
#include "print_asm.h"


int lineno;

body *theprogram;

int main(int argc, char **argv) {
    int helpflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;
    a_asm *program;

    opterr = 0;

    int files[argc];

    while ((c = getopt(argc, argv, "hc:")) != -1) {
        switch (c) {
        case 'h':
            helpflag = 1;
            files[optind - 2] = 1;
            break;
        case 'c':
            cvalue = optarg;
            files[optind - 3] = 1;
            files[optind - 2] = 1;
            break;
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            return 1;
        default:
            abort();
        }
    }
    if (helpflag) {
        system("man ./manual");
        return 0;
    }

    if (optind < argc) {
        for (int i = 1; i < argc; ++i) {
            if (files[i] == 0) {
                if (ends_with(argv[i], ".src")) {
                    freopen(argv[i], "r", stdin);
                }
            }
        }
    }

    lineno = 1;
    yyparse();
    
    //printf("\nStarting weeder\n\n");
    weeder_init(theprogram);
    types = 0;
    //prettyProgram(theprogram);


    //printf("\nStarting typechecking\n\n");

#if debugflag > 0
    printf("\nStarting typechecking\n\n");
#endif

    typecheck(theprogram);

#if debugflag > 0
    printf("\nAfter typechecking\n\n");
#endif

    types = 1;
    prettyProgram(theprogram);


    program = generate_program(theprogram);
    print_asm(program, "a.s");


    printf("\n");
    return 1;
}
