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
#include "reg_alloc.h"
#include "peephole.h"
#include "bit_vector.h"
#include "liveness_analysis.h"

int lineno;
extern int verbose = 0;

body *theprogram;

int main(int argc, char **argv) {
    int helpflag = 0;
    int assemble_flag = 0;
    int bflag = 0;
    int printpeep = 0;
    int prettyprint = 0;

    char *cvalue = NULL;
    char *directory = "./";
    char *filename;
    int index;
    int c;
    a_asm *program;

    opterr = 0;

    int files[argc];

    while ((c = getopt(argc, argv, "ho:avmp:")) != -1) {
        switch (c) {
        case 'h':
            helpflag = 1;
            files[optind - 2] = 1;
            break;
        case 'a':
            assemble_flag = 1;
            files[optind - 2] = 1;
            break;
        case 'o':
            directory = optarg;
            files[optind - 3] = 1;
            files[optind - 2] = 1;
            break;
        // case 'c':
        // 	filename = get_filename(optarg, ".src");
        // 	freopen(optarg, "r", stdin);
        // 	break;
        case 'm':
            printpeep = 1;
            files[optind - 2] = 1;
            break;
        case 'p':
            prettyprint = atoi(optarg);
            files[optind - 3] = 1;
            files[optind - 2] = 1;
            break;
        case 'v':
            verbose = 1;
            files[optind - 2] = 1;
            break;
        case '?':
            printf("Unknown option, try again.\n");
            return 1;
        default:
            abort();
        }
    }

    if (helpflag) {
        system("man ./manual");
        return 0;
    }

    printf("Num opts: %i, Num args: %i\n", optind, argc);
    if (optind < argc) {
        for (int i = 1; i < argc; ++i) {
            if (files[i] == 0) {
                if (ends_with(argv[i], ".src")) {
                    filename = get_filename(argv[i], ".src");
                    freopen(argv[i], "r", stdin);
                    break;
                }
            }
        }
    }

    lineno = 1;
    yyparse();

    if (verbose) {
        printf("\nStarting weeder\n\n");
    }

    if (theprogram == NULL) {
        printf("You appear to be missing any function calls\n");
        return 0;
    }
    weeder_init(theprogram);

    if (prettyprint == 1) {

        types = 0;
        prettyProgram(theprogram);
    }

    if (verbose) {
        printf("\nStarting typechecking\n\n");
    }

    typecheck(theprogram);

    if (prettyprint == 2) {
        types = 1;
        prettyProgram(theprogram);
    }

    if (verbose) {
        printf("\nStarting code generation\n\n");
    }

    program = generate_program(theprogram);

    if (printpeep) {
        if (verbose) {
            printf("Printing asm without peep\n");
        }
        print_asm(program, concat_string(directory, concat_string(filename, "_nopeep.s")));
    }

    if (verbose) {
        printf("First peephole\n");
    }

    peephole(program);

    if (printpeep) {
        if (verbose) {
            printf("Printing asm with peep\n");
        }
        print_asm(program, concat_string(directory, concat_string(filename, "_peep.s")));
    }

    if (verbose) {
        printf("Starting Liveness analysis and Register allocation\n");
    }

    program = reg_alloc(program);

    if (verbose) {
        printf("Second peephole\n");
    }

    peephole(program);

    if (verbose) {
        printf("Outputting final program\n");
    }

    print_asm(program, concat_string(directory, concat_string(filename, ".s")));

    if (assemble_flag == 1) {
        // int len = strlen("gcc -o ");
        // char *compile_string[1024];
        // snprintf(compile_string, 1024, "gcc -o %s %s%s_a2.s",filename,directory,filename);

        // printf("%s\n",compile_string);
		char* compile_string = concat_string("gcc -no-pie -o ", filename);
		compile_string = concat_string(compile_string, ".out ");
		compile_string = concat_string(compile_string, directory);
		compile_string = concat_string(compile_string, filename);
		compile_string = concat_string(compile_string, ".s");
		//printf("\n%s\n",compile_string);
		system(compile_string);
    }

    printf("\n");
    return 1;
}
