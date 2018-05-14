#include <stdio.h>
#include <string.h>
#include "rewriter.h"
#include "reg_alloc.h"
#include "bit_vector.h"
#include "memory.h"
#include "error.h"
#include "bit_vector.h"
#include "main.h"

//Made separate file for the rewriters helper functions, since they filled a lot

//Final rewrite of the program, replaced temporaries with proper registers
a_asm *final_rewrite(int *colors, a_asm *program){

    struct a_asm *temp;
    struct a_asm *func_start;
    struct a_asm *begin_call;
    struct a_asm *call;
    struct a_asm *temp2;

    int stack_offset;
    int line;
    temp = program;
    stack_offset = 0;
    line = 1;

    if (verbose){
        printf("\nFinal rewriting of program\n\n");
    }

    while (temp != NULL){
        
        switch (temp->ins){

            case (LABEL):
                //Check what kind of label we found
                switch (temp->val.label.type){
                    //Found start of function, save this location for later
                    case (label_FUNC_START):
                        func_start = temp;
                        stack_offset = temp->val.label.func_vars;
                        break;

                    //Found end of function, rewrite pre- and postfix of function
                    case (label_FUNC_END):
                        temp = rewrite_function(func_start, temp, stack_offset);
                        func_start = NULL;
                        break;

                    case (label_NORMAL):
                        break;


                }
            break;


            case (MOVQ):
            case (ADDQ):
            case (LEAQ):
            case (SUBQ):
            case (XORQ):
            case (SARQ):
            case (CMP):
                rewrite_op(colors, &temp->val.two_op.op1, &stack_offset);
                rewrite_op(colors, &temp->val.two_op.op2, &stack_offset);
                break;

            case (IMUL):
            case (IDIV):
                rewrite_op(colors, &temp->val.one_op.op, &stack_offset);
                break;

            case (PUSH):
            case (POP):
                rewrite_op(colors, &temp->val.one_op.op, &stack_offset);
                break;

            case (CALL):
                call = temp;
                temp2 = add_push_of_live(program, colors, call);
                asm_insert(&temp, &begin_call, &temp2);
                break;

            case (BEGIN_CALL):
                //Save location of the beginning of the call
                begin_call = temp;
                break;

            case (END_CALL):
                //Restore saved registers
                temp2 = add_pop_of_live(program, colors, call);
                asm_insert(&temp, &temp, &temp2);
                
                call = NULL;
                break;

        }




        temp = temp->next;
        line++;
    }
    return program;
}


void rewrite_op(int *colors, asm_op **op, int *stack_offset){
    struct asm_op *new_reg;
    int temp;
    
    if ((*op)->type == op_SPILL){
        if ((*op)->stack_offset == -1){
            (*stack_offset)++;
            (*op)->stack_offset = (*stack_offset);
        }

    } else {
        temp = get_reg((*op));
        if (temp != -1){    
            if (!is_precolored(temp)){
                new_reg = get_corresponding_reg(colors[temp]);
                replace_temp_op(op, new_reg);
            }
        }
    }

    

}

//Add pre- and postfix to a function
a_asm *rewrite_function(a_asm *func_start, a_asm *func_end, int offset){
    struct a_asm *head;
    struct a_asm *tail;
    struct a_asm *pre;
    struct a_asm *post;

    head = func_start;
    tail = func_start;

    pre = add_prefix(offset);

    asm_insert(&head, &tail, &pre);

    tail = func_end;

    post = add_postfix(offset);

    asm_insert(&head, &tail, &post);

    return tail;

}


a_asm *add_prefix(int offset){
    struct a_asm *head;
    struct a_asm *tail;

    head = NULL;
    tail = NULL;

    add_1_ins(&head, &tail, PUSH, reg_RBP, "Push old base pointer to stack");
    add_2_ins(&head, &tail, MOVQ, reg_RSP, reg_RBP, "Move stack pointer to base pointer");

    //Add space for variables and spills in function

    if (offset != 0){
        add_2_ins(&head, &tail, SUBQ, make_op_const(offset * 8), reg_RSP, "Make space for variables and spills");
    }

    return head;
}

a_asm *add_postfix(int offset){
    struct a_asm *head;
    struct a_asm *tail;

    head = NULL;
    tail = NULL;

    if (offset != 0){
        add_2_ins(&head, &tail, ADDQ, make_op_const(offset * 8), reg_RSP, "Remove space for variables and spills");
    }

    add_2_ins(&head, &tail, MOVQ, reg_RBP, reg_RSP, "Restore old stack pointer");
    add_1_ins(&head, &tail, POP, reg_RBP, "Restore old base pointer");

    return head;

}

a_asm *add_pop_of_live(a_asm *program, int *colors, a_asm *call){
    struct a_asm *head;
    struct a_asm *tail;
    int *regs;
    int *popped_regs[AVAIL_REGS];
    head = NULL;
    tail = NULL;

    regs = get_used_regs(program, call->val.one_op.op->val.label_id, colors);
    
    for (int i = 0; i < AVAIL_REGS; i++){
        popped_regs[i] = 1;
    }

    //Want to restore backwards
    for (int j = temps-1; j >= 0; j--){
        if (get_bit(call->new, j) && regs[colors[j]] && popped_regs[colors[j]]){
            popped_regs[colors[j]] = 0;
            add_1_ins(&head, &tail, POP, get_corresponding_reg(colors[j]), "Register was live in function, so restoring it after CALL");
        }
    }
    
    return head;


}

a_asm *add_push_of_live(a_asm *program, int *colors, a_asm *call){
    struct a_asm *head;
    struct a_asm *tail;
    int *regs;
    int *pushed_regs[AVAIL_REGS];
    head = NULL;
    tail = NULL;

    regs = get_used_regs(program, call->val.one_op.op->val.label_id, colors);
    
    for (int i = 0; i < AVAIL_REGS; i++){
        pushed_regs[i] = 1;
    }
    
    for (int j = 0; j < temps; j++){
        if (get_bit(call->new, j) && regs[colors[j]] && pushed_regs[colors[j]]){
            pushed_regs[colors[j]] = 0;
            add_1_ins(&head, &tail, PUSH, get_corresponding_reg(colors[j]), "Register is live in function, so saving it before CALL");
        }
    }
    
    return head;


}

int *get_used_regs(a_asm *program, char *function, int *colors){
    int *regs;

    regs = malloc(sizeof(int) * AVAIL_REGS);

    //Zero initialize used regs;
    for (int i = 0; i < AVAIL_REGS; i++){
        regs[i] = 0;
    }

    //prinf is a special case, since we know what it overrides
    if (strcmp(function, "printf") == 0){
        regs[0] = 1;
        regs[2] = 1;
        regs[3] = 1;
        return regs;
    }

    //Search for function in program
    while (program != NULL){
        if (program->ins == LABEL && program->val.label.type == label_FUNC_START){
            if (strcmp(function, program->val.label.label_id) == 0){

                //Found function we are looking for, search for temps in the function, and set their allocated register to be used
                while (program != NULL && program->ins != RET){
                    
                    for (int j = 0; j < temps; j++){
                        if (program->def != NULL){
                            if (get_bit(program->def, j)){
                                //printf("Temp %d is defined in function %s, color: %d\n", j, function, colors[j]);

                                regs[colors[j]] = 1;

                            }
                        }
                    }
                    program = program->next;

                }
                return regs;


            }
        }

        program = program->next;
    }

    return NULL;

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