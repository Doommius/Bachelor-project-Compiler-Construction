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
               exp_TERM } EXP_kind;

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
               statement_FOR,
               statement_LIST } STATEMENT_kind;

typedef enum { var_ID,
               var_EXP,
               var_RECORD } Var_kind;

typedef enum { al_LIST,
               al_EMPTY } AL_kind;

typedef enum { el_EXP,
               el_LIST } EL_kind;

typedef enum { 
               LABEL,
               JMP, //unconditional jump
               JE,  //jump equal
               JNE, //Jump not equal
               JG,  //jump greater
               JGE, //jump greater or equal
               JL,  //Jump less
               JLE, //Jump less or equal
               IMUL,                //One op
               IDIV,                //One op
               MOVQ,                //Two op
               ADDQ,                //Two op
               SUBQ,                //Two op
               XORQ,
               SARQ,
               CALL,
               RET,
               LEAQ,

               PUSH,                //One op
               POP,                 //One op
               ANDQ, //Just for testing

               CMP,                 //Two op
               CDQ,
               BEGIN_CALL,          //Used to signal the beginning of a call instruction, meaning we have to save registers
               END_CALL             //Used to signal the end of a call instruction, meaning we have to retore registers
} ASM_kind;

typedef enum { op_INTCONST,
               op_REGISTER,
               op_STACK_LOC,
               op_MEM_LOC,
               op_LABEL,
               op_CONST,
               op_LEA,

               op_TEMP, //Temporary until we find out what register/stack location it should be in, atleast i hope so.
               op_SPILL

} OP_kind;

typedef enum { label_NORMAL,
               label_FUNC_START,
               label_FUNC_END

} LABEL_kind;

#endif //COMPILER_KIND_H