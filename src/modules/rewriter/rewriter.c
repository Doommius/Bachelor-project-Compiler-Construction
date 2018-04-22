#include <stdio.h>
#include "rewriter.h"
#include "reg_alloc.h"
#include "bit_vector.h"

//Made separate file for the rewriters helper functions, since they filled a lot

//Final rewrite of the program, replaced temporaries with proper registers
a_asm *final_rewrite(int *colors, a_asm *program){

    struct a_asm *temp;
    temp = program;
    while (temp != NULL){
        
        switch (temp->ins){

            case (MOVQ):
            case (ADDQ):
            case (SUBQ):
            case (CMP):
                rewrite_op(colors, &temp->val.two_op.op1);
                rewrite_op(colors, &temp->val.two_op.op2);
                break;

            case (IMUL):
            case (IDIV):
                rewrite_op(colors, &temp->val.one_op.op);
                break;





        }




        temp = temp->next;
    }
    return program;
}


void rewrite_op(int *colors, asm_op **op){
    struct asm_op *new_reg;
    int temp = get_reg((*op));
    if (temp != -1){
        if (!is_precolored(temp)){
            new_reg = get_corresponding_reg(colors[temp]);
            (*op) = new_reg;
        }
    }

    
}

//Returns the register corresponding to the given number
asm_op *get_corresponding_reg(int reg){

    switch (reg){
        case (0):
            return reg_RAX;
            break;
        
        case (1):
            return reg_RBX;
            break;
        
        case (2):
            return reg_RCX;
            break;
        
        case (3):
            return reg_RDX;
            break;
        
        case (4):
            return reg_RSI;
            break;
        
        case (5):
            return reg_RDI;
            break;
        
        case (6):
            return reg_R8;
            break;
        
        case (7):
            return reg_R9;
            break;
        
        case (8):
            return reg_R10;
            break;

        case (9):
            return reg_R11;
            break;
        
        case (10):
            return reg_R12;
            break;
        
        case (11):
            return reg_R13;
            break;
        
        case (12):
            return reg_R14;
            break;
        
        case (13):
            return reg_R15;
            break;

        default:
            break;
    }

    return NULL;

        
}