#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "weeder.h"
#include "error.h"

body *theprogram;
/**
 * What we want to weed (TO BE EXPANDED):
 *      [IF] statements:
 *          true && term    - should be term
 *          false && term   - should be false
 *          true || term    - should be true
 *          false || term   - should be true
 *          break/continue  - error (for now, break could possibly be implemented)
 *      
 *      [CONSTANTS]
 *          F.x. 5*2        - should be 10, can be calculated during compile time
 * 
 *      [FUNCTIONS]
 *          no return       - error
 *          
 * 
 */

body *weeder(body *program){
    //TODO Skal nok vaere noget mere her
    program = weed_body(program);
    return program;

}

body *weed_body(body *body){
    body->d_list = weed_dlist(body->d_list);
    body->s_list = weed_slist(body->s_list);
    return body;
}

function *weed_function(function *func){
    func->body = weed_body(func->body);
    func->head = weed_head(func->head);
    func->tail = weed_tail(func->tail);
    return func;

}

head *weed_head(head *head){
    head->list = weed_pdl(head->list);
    head->type = weed_type(head->type);
    return head;

}

//TODO Could be remove, pretty much useless
tail *weed_tail(tail *tail){
    return tail;
}

type *weed_type(type *type){
    switch (type->kind){
        case (type_ARRAY):
            type->val.type = weed_type(type->val.type);
            break;
        
        case (type_RECORD):
            type->val.list = weed_vdl(type->val.list);
            break;

        default:
            break;
    }
    return type;

}

par_decl_list *weed_pdl(par_decl_list *pdl){
    if (pdl->kind == pdl_EMPTY){
        return pdl;
    }

    pdl->list = weed_vdl(pdl->list);
    return pdl;
}

var_decl_list *weed_vdl(var_decl_list *vdl){
    switch (vdl->kind){

        case (vdl_TYPE):
            vdl->vartype = weed_vtype(vdl->vartype);
            break;

        case (vdl_LIST):
            vdl->list = weed_vdl(vdl->list);
            break;

    }
    return vdl;

}

var_type *weed_vtype(var_type *vtype){

    vtype->type = weed_type(vtype->type);
    return vtype;

}

decl_list *weed_dlist(decl_list *dlist){
    if (dlist->kind == dl_EMPTY){
        //Nothing to do
        return dlist;
    }
    dlist->list = weed_dlist(dlist->list);
    dlist->decl = weed_decl(dlist->decl);
    return dlist;

}

declaration *weed_decl(declaration *decl){
    switch (decl->kind){

        case (decl_TYPE):
            decl->val.type.type = weed_type(decl->val.type.type);
            break;
        
        case (decl_FUNC):
            decl->val.function = weed_function(decl->val.function);
            break;

        case (decl_VAR):
            decl->val.list = weed_vdl(decl->val.list);
            break;

    }
    return decl;

}

statement_list *weed_slist(statement_list *slist){

    

}

statement *weed_stmt(statement *stmt){

}

variable *weed_variable(variable *variable){
    switch (variable->kind){

        case (var_EXP):
            variable->val.exp.exp = weed_expression(variable->val.exp.exp);
            variable->val.exp.var = weed_variable(variable->val.exp.var);
            break;

        case (var_RECORD):
            variable->val.record.var = weed_variable(variable->val.exp.var);
            break;

        default:
            break;

    }
    return variable;


}

expression *weed_expression(expression *expression){

    struct expression *left_exp;
    struct expression *right_exp;

    struct term *left_term;
    struct term *right_term;
    struct term *temp;

    if (expression->kind == exp_TERM){
        expression->val.term = weed_term(expression->val.term);
        return expression;
    }

    expression->val.ops.left = weed_expression(expression->val.ops.left);
    expression->val.ops.right = weed_expression(expression->val.ops.right);

    left_exp = expression->val.ops.left;
    right_exp = expression->val.ops.right;

    if ((left_exp->kind == exp_TERM) && (right_exp->kind == exp_TERM)){
        left_term = left_exp->val.term;
        right_term = right_exp->val.term;
        if ((left_term->kind == term_NUM) && (right_term->kind == term_NUM)){
            //We have an expression with two constants
            switch(expression->kind){

                case (exp_MULT):
                    temp = make_Term_num(left_term->val.num * right_term->val.num);
                    break;

                case (exp_DIV):
                    if(right_term->val.num == 0){
                        print_error("Division by 0 error", 1, right_term->lineno);
                    }
                    temp = make_Term_num(left_term->val.num / right_term->val.num);
                    break;

                case (exp_PLUS):
                    temp = make_Term_num(left_term->val.num + right_term->val.num);
                    break;

                case (exp_MIN):
                    temp = make_Term_num(left_term->val.num - right_term->val.num);
                    break;

                case (exp_EQ):
                    if (left_term->val.num == right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;

                case (exp_NEQ):
                    if (left_term->val.num != right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;

                case (exp_GT):
                    if (left_term->val.num > right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;

                case (exp_LT):
                    if (left_term->val.num < right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;

                case (exp_GEQ):
                    if (left_term->val.num >= right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;

                case (exp_LEQ):
                    if (left_term->val.num <= right_term->val.num){
                        temp = make_Term_boolean(1);
                    } else {
                        temp = make_Term_boolean(0);
                    }
                    break;
                
            }
        }

        //TODO Optimize this please, to many comparisons I think, or maybe put advanced patterns into a function for itself?
        if (temp == NULL){
            if (expression->kind == exp_AND){

                if ((left_term->kind == term_FALSE) || (right_term->kind == term_FALSE)){
                    temp = make_Term_boolean(0);
                }
                if ((left_term->kind == term_TRUE) && (right_term->kind == term_TRUE)){
                    temp = make_Term_boolean(1);
                }

                //Advanced patterns

                if (left_exp->kind == exp_TERM){
                    if (left_exp->val.term->kind == term_TRUE){
                        return expression->val.ops.right;
                    }
                    if (left_exp->val.term->kind == term_FALSE){
                        temp = make_Term_boolean(1);
                    }
                }

                if (right_exp->kind == exp_TERM){
                    if (right_exp->val.term->kind == term_TRUE){
                        return expression->val.ops.left;
                    }
                    if (right_exp->val.term->kind == term_FALSE){
                        temp = make_Term_boolean(1);
                    }
                }
                
            }

            if (expression->kind == exp_OR){
                if ((left_term->kind == term_TRUE) || (right_term->kind == term_TRUE)){
                    temp = make_Term_boolean(1);
                }
                if ((left_term->kind == term_FALSE) && (right_term->kind == term_FALSE)){
                    temp = make_Term_boolean(0);
                }

                //Advanced patterns

                if (left_exp->kind == exp_TERM){
                    if (left_exp->val.term->kind == term_FALSE){
                        return expression->val.ops.right;
                    }
                    if (left_exp->val.term->kind == term_TRUE){
                        temp = make_Term_boolean(1);
                    }
                }

                if (right_exp->kind == exp_TERM){
                    if (right_exp->val.term->kind == term_FALSE){
                        return expression->val.ops.left;
                    }
                    if (right_exp->val.term->kind == term_TRUE){
                        temp = make_Term_boolean(1);
                    }
                }
            }
        }

        if (temp != NULL){
            //We reduced something
            expression->kind = exp_TERM;
            expression->val.term = temp;
        }
    }

    return expression;
}

term *weed_term(term *term){

}

act_list *weed_alist(act_list *alist){
    if (alist->kind == al_EMPTY){
        return alist;
    }
    alist->list = weed_elist(alist->list);
    return alist;

}

exp_list *weed_elist(exp_list *elist){

    elist->expression = weed_expression(elist->expression);
    
    if (elist->kind == el_LIST){
        elist->list = weed_elist(elist->list);
    }

    return elist;

}