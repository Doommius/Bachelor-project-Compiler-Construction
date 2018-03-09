#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

#include "main.h"
#include "auxiliary.h"
#include "symbol.h"
#include "tree.h"
#include "pretty.h"
#include "y.tab.h"
#include "weeder.h"
#include "typechecker.h"


int lineno;

body *theprogram;

int main(int argc, char **argv) {
    int helpflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;

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
    printf("Before weeding:\n");
    prettyBody(theprogram);

    //weeder(theprogram);

    printf("Starting typechecking\n");
    typecheck(theprogram);

    printf("\nAfter weeding:\n");
    prettyBody(theprogram);

    printf("\n");
    return 1;
}
