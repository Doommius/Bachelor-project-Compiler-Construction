#ifndef __compiler_kind_h
#define __compiler_kind_h

typedef enum { exp_PLUS,
               exp_MIN,
               exp_MULT,
               exp_DIV,
               exp_EQ,
               exp_NEQ,
               exp_GT,
               exp_LT,
               exp_GEQ,
               exp_LEQ,
               exp_AND,
               exp_OR,
               exp_TERM} EXP_kind;

typedef enum { term_VAR,
               term_LIST,
               term_PAR,
               term_NOT,
               term_ABS,
               term_NUM,
               term_TRUE,
               term_FALSE,
               term_NULL } TERM_kind;

typedef enum { type_ID,
               type_INT,
               type_BOOl,
               type_ARRAY,
               type_RECORD } TYPE_kind;

typedef enum { pdl_LIST,
               pdl_EMPTY } PDL_kind;

typedef enum { vdl_LIST,
               vdl_TYPE } VDL_kind;

typedef enum { dl_LIST,
               dl_EMPTY } DL_kind;

typedef enum { decl_TYPE,
               decl_FUNC,
               decl_VAR } DECL_kind;

typedef enum { sl_STATEMENT,
               sl_LIST } SL_kind;

typedef enum { statement_RETURN,
               statement_WRITE,
               statement_ALLOCATE,
               statement_ALLOCATE_LENGTH,
               statement_ASSIGNMENT,
               statement_IF,
               statement_IF_ELSE,
               statement_WHILE,
               statement_LIST } STATEMENT_kind;

typedef enum { var_ID,
               var_EXP,
               var_VID } Var_kind;

typedef enum { al_LIST,
               al_EMPTY } AL_kind;

typedef enum { el_EXP,
               el_LIST } EL_kind;


#endif //COMPILER_KIND_H