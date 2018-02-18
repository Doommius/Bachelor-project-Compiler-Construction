/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tINTCONST = 258,
    tIDENTIFIER = 259,
    EQ = 260,
    NEQ = 261,
    LEQ = 262,
    GEQ = 263,
    LT = 264,
    GT = 265,
    IF = 266,
    ELSE = 267,
    WHILE = 268,
    FOR = 269,
    IN = 270,
    FUNCTION = 271,
    TYPEINT = 272,
    TYPESTRING = 273,
    TYPEBOOLEAN = 274,
    SEMICOLON = 275,
    RETURN = 276,
    AND = 277,
    OR = 278,
    ASSIGN = 279,
    TRUE = 280,
    FALSE = 281,
    _NULL = 282,
    FUNC = 283,
    END = 284,
    INT = 285,
    BOOL = 286,
    ARRAY_OF = 287,
    RECORD_OF = 288,
    TYPE = 289,
    VAR = 290,
    OF_LENGTH = 291,
    THEN = 292,
    WRITE = 293,
    ALLOCATE = 294,
    DO = 295
  };
#endif
/* Tokens.  */
#define tINTCONST 258
#define tIDENTIFIER 259
#define EQ 260
#define NEQ 261
#define LEQ 262
#define GEQ 263
#define LT 264
#define GT 265
#define IF 266
#define ELSE 267
#define WHILE 268
#define FOR 269
#define IN 270
#define FUNCTION 271
#define TYPEINT 272
#define TYPESTRING 273
#define TYPEBOOLEAN 274
#define SEMICOLON 275
#define RETURN 276
#define AND 277
#define OR 278
#define ASSIGN 279
#define TRUE 280
#define FALSE 281
#define _NULL 282
#define FUNC 283
#define END 284
#define INT 285
#define BOOL 286
#define ARRAY_OF 287
#define RECORD_OF 288
#define TYPE 289
#define VAR 290
#define OF_LENGTH 291
#define THEN 292
#define WRITE 293
#define ALLOCATE 294
#define DO 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "bison/exp.y" /* yacc.c:1909  */

    int intconst;
    char *stringconst;
    struct EXP *exp;
    struct function *function;
    struct head *head;
    struct tail *tail;
    struct type *type;
    struct par_decl_list *par_decl_list;
    struct var_decl_list *var_decl_list;
    struct var_type *var_type;
    struct body *body;
    struct decl_list *decl_list;
    struct declaration *declaration;
    struct statement_list *statement_list;
    struct statement *statement;
    struct variable *variable;
    struct expression *expression;
    struct term *term;
    struct act_list *act_list;
    struct exp_list *exp_list;

#line 157 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
