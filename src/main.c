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

body *theprogram;

int main(int argc, char **argv)
{
	int helpflag = 0;
	int assemble_flag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	char *directory = "./";
	char *filename;
	int index;
	int c;
	a_asm *program;

	opterr = 0;

	int files[argc];

	while ((c = getopt(argc, argv, "hc:o:a")) != -1)
	{
		switch (c)
		{
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
			files[optind - 2] = 1;
			break;
		case 'c':
			filename = get_filename(optarg, ".src");
			freopen(optarg, "r", stdin);
			break;
		case '?':
			printf("Unknown option, try again.\n");
			return 1;
		default:
			abort();
		}
	}
	if (helpflag)
	{
		system("man ./manual");
		return 0;
	}

	printf("Num opts: %i, Num args: %i\n", optind, argc);
	if (optind < argc)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (files[i] == 0)
			{
				if (ends_with(argv[i], ".src"))
				{
					filename = get_filename(argv[i], ".src");
					freopen(argv[i], "r", stdin);
				}
			}
		}
	}

	lineno = 1;
	yyparse();

	printf("\nStarting weeder\n\n");

	if (theprogram == NULL)
	{
		printf("You appear to be missing any function calls\n");
		return 0;
	}

	printf("\nAt least one function call\n\n");
	weeder_init(theprogram);
	types = 0;
	//prettyProgram(theprogram);

	printf("\nStarting typechecking\n\n");

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

	printf("Printing asm without peep\n");

	print_asm(program, concat_string(directory, concat_string(filename, "_nopeep.s")));

	printf("Peepholing\n");
	peephole(program);

	printf("Printing asm with peep\n");

	print_asm(program, concat_string(directory, concat_string(filename, "_peep.s")));

	printf("Starting Register allocation\n");
	//test_known_program();
	program = reg_alloc(program);

	print_asm(program, concat_string(directory, concat_string(filename, "_a2.s")));

	peephole(program);

	print_asm(program, concat_string(directory, concat_string(filename, "_a3.s")));

	if (assemble_flag == 1)
	{
		int len = strlen("gcc -o ");
		char *compile_string[1024];
		snprintf(compile_string, 1024, "gcc -o %s %s%s_a2.s",filename,directory,filename);

		printf("%s\n",compile_string);
		// system(concat_string("gcc -o ", concat_string(filename,concat_string(" ",
		// 											  concat_string(directory,
		// 															concat_string(filename, "_a2.s"))))));
		system(compile_string);
	}

	printf("\n");
	return 1;
}
