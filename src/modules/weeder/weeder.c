#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "weeder.h"
#include "error.h"
#include "stack.h"

body *theprogram;

struct stack *function_stack;

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


body *weeder_init(body *program){

    function_stack = init_stack();
    program = weed_body(program);
    return program;


}

body *weed_body(body *body){
    body->d_list = weed_dlist(body->d_list);
    body->s_list = weed_slist(body->s_list);
    return body;
}

function *weed_function(function *func){
	
    stack_push(function_stack, func);

    func->body = weed_body(func->body);
    
    if (func->body->s_list->contains_ret == 0){
        print_error("Function does not contain a return", 0, func->lineno);
    }

    func->head = weed_head(func->head);

    stack_pop(function_stack);

    return func;

}

head *weed_head(head *head){
    head->list = weed_pdl(head->list);
    head->type = weed_type(head->type);
    return head;

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
    //printf("Weeding slist\n");
    struct function *f;
    slist->contains_ret = 0;

    if (slist == NULL){
        return NULL;
    }

    slist->statement = weed_stmt(slist->statement);
    slist->contains_ret = slist->statement->contains_ret;

    if (slist->kind == sl_LIST){
        slist->list = weed_slist(slist->list);
        slist->contains_ret = slist->list->contains_ret;
    }

    
    if (slist->statement == NULL){
        return slist->list;
    }

    f = stack_read(function_stack);

    if (f != NULL){
        if (slist->statement->kind == statement_IF_ELSE){
            if (slist->statement->contains_ret == 0){
                // prettySTMT(slist->statement);
                print_error("If-else does not contain enough returns", 0, slist->statement->lineno);
            }
        }
        
        //We need to check if there is a return after the if
        if (slist->statement->kind == statement_IF){
            if (slist->list->contains_ret == 0){
                print_error("No return after IF", 0, slist->statement->lineno);
                
            }
            
        }
        // printf("Slist's statement contains a ret? %d\n", slist->statement->contains_ret);

        // prettySTMT(slist->statement);
        // slist->contains_ret = slist->statement->contains_ret;
        f->body->s_list->contains_ret = 1;
    }

    


    return slist;

}

statement *weed_stmt(statement *stmt){

    struct function *f;

    switch(stmt->kind){

        case (statement_RETURN):
            //printf("Statement return\n");
            //printf("Before weeding expression:\n");
            //prettyEXP(stmt->val.ret);
            //printf("\n\n");
            stmt->val.ret = weed_expression(stmt->val.ret);
            //printf("After weeding expression:\n");
            //prettyEXP(stmt->val.ret);
            //printf("\n\n");

            f = stack_read(function_stack);
            //printf("Function this return belongs to: %s", f->head->id);
            if (f == NULL){
                print_error("Return outside of function", 0, stmt->lineno);
            }
            stmt->function = f;
            stmt->contains_ret = 1;
            break;

        case (statement_WRITE):
            //printf("Statement write\n");
            stmt->val.wrt = weed_expression(stmt->val.wrt);
            break;

        case (statement_ALLOCATE):
            //printf("Statement allocate\n");
            stmt->val.allocate.variable = weed_variable(stmt->val.allocate.variable);
            break;

        case (statement_ALLOCATE_LENGTH):
            //printf("Statement allocate length\n");
            stmt->val.allocate.variable = weed_variable(stmt->val.allocate.variable);
            stmt->val.allocate.length = weed_expression(stmt->val.allocate.length);
            break;

        case (statement_ASSIGNMENT):
            //printf("Statement assignment\n");
            stmt->val.assignment.variable = weed_variable(stmt->val.assignment.variable);
            stmt->val.assignment.expression = weed_expression(stmt->val.assignment.expression);
            break;

        case (statement_IF):
            //printf("Statement if\n");
            stmt->val.ifthen.expression = weed_expression(stmt->val.ifthen.expression);
            stmt->val.ifthen.statement1 = weed_stmt(stmt->val.ifthen.statement1);

            //Check for return in if
            if (stmt->val.ifthen.statement1->contains_ret == 1){
                stmt->contains_ret = 1;
            }

            // Check if expression is always true/false
            if (stmt->val.ifthen.expression->kind == exp_TERM){
                if (stmt->val.ifthen.expression->val.term->kind == term_FALSE){
                    return NULL;
                }
                stmt = stmt->val.ifthen.statement1;
            }
            break;

        case (statement_IF_ELSE):
            stmt->val.ifthen.expression = weed_expression(stmt->val.ifthen.expression);
            stmt->val.ifthen.statement1 = weed_stmt(stmt->val.ifthen.statement1);
            //printf("Weeding statement 2\n");
            stmt->val.ifthen.statement2 = weed_stmt(stmt->val.ifthen.statement2);
            //printf("Done weeding statement 2\n");
            
            if (stmt->val.ifthen.statement1->contains_ret == 1 && stmt->val.ifthen.statement2->contains_ret == 1){
                stmt->contains_ret = 1;
            }

            //Check for return in both if and else
            
            if (stmt->val.ifthen.statement1->contains_ret == 1 && stmt->val.ifthen.statement2->contains_ret == 1){
                stmt->contains_ret = 1;
            }

            // Check if expression is always true/false
            if (stmt->val.ifthen.expression->kind == exp_TERM){
                if (stmt->val.ifthen.expression->val.term->kind == term_FALSE){
                    stmt = stmt->val.ifthen.statement2;
                } else {
                    stmt = stmt->val.ifthen.statement1;
                }
            }
            
            break;

        case (statement_WHILE):
            //printf("Statement while\n");
            stmt->val.loop.expression = weed_expression(stmt->val.loop.expression);
            stmt->val.loop.statement = weed_stmt(stmt->val.loop.statement);
            if (stmt->val.loop.statement == NULL){
                return NULL;
            }
            break;

        case (statement_LIST):
            //printf("Statement list\n");
            stmt->val.list = weed_slist(stmt->val.list);
            if (stmt->val.list == NULL){
                return NULL;
            }
            stmt->contains_ret = stmt->val.list->contains_ret;
            break;
    }
        
    return stmt;

}

variable *weed_variable(variable *variable){
    switch (variable->kind){

        case (var_ID):
            break;


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

    temp = NULL;


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

        //Check for boolean expression
        switch(expression->kind){

            case (exp_AND):
                if ((left_term->kind == term_FALSE) || (right_term->kind == term_FALSE)){
                    temp = make_Term_boolean(0);
                }

                if ((left_term->kind == term_TRUE) && (right_term->kind == term_TRUE)){
                    temp = make_Term_boolean(1);
                }

                break;

            case (exp_OR):
                if ((left_term->kind == term_TRUE) || (right_term->kind == term_TRUE)){
                    temp = make_Term_boolean(1);
                }

                if ((left_term->kind == term_FALSE) && (right_term->kind == term_FALSE)){
                    temp = make_Term_boolean(0);
                }
                break;

            default:
                break;
        }

        //TODO Optimize this please, to many comparisons I think, or maybe put advanced patterns into a function for itself?
        if (temp == NULL){

            if (expression->kind == exp_AND){

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
    }
    
    if (temp != NULL){
        //We reduced something
        expression->kind = exp_TERM;
        expression->val.term = temp;
    }

    return expression;
}

term *weed_term(term *term){
    struct expression *e;

    switch(term->kind){
        case (term_VAR):
            term->val.variable = weed_variable(term->val.variable);
            break;
        
        case (term_LIST):
            term->val.list.list = weed_alist(term->val.list.list);
            break;

        case (term_PAR):
            term->val.expression = weed_expression(term->val.expression);
            break;

        case (term_NOT):
            term->val.term_not = weed_term(term->val.term_not);
            if (term->val.term_not->kind == term_TRUE){
                term->kind = term_FALSE;
                break;
            }

            if (term->val.term_not->kind == term_FALSE){
                term->kind = term_TRUE;
                break;
            }

            if (term->val.term_not->kind == term_NOT){
                term = term->val.term_not->val.term_not;
                break;
            }

            if (term->val.term_not->kind == term_PAR){
                e = term->val.term_not->val.expression;
                switch(e->kind){
                    
                    case (exp_EQ):
                        e->kind = exp_NEQ;
                        term = term->val.term_not;
                        break;

                    case (exp_NEQ):
                        e->kind = exp_EQ;
                        term = term->val.term_not;
                        break;

                    case (exp_LT):
                        e->kind = exp_GEQ;
                        term = term->val.term_not;
                        break;

                    case (exp_GT):
                        e->kind = exp_LEQ;
                        term = term->val.term_not;
                        break;

                    case (exp_LEQ):
                        e->kind = exp_GT;
                        term = term->val.term_not;
                        break;

                    case (exp_GEQ):
                        e->kind = exp_LT;
                        term = term->val.term_not;
                        break;

                    default:
                        break;

                }
            }
            break;

        case (term_ABS):
            term->val.expression = weed_expression(term->val.expression);
            if ((term->val.expression->kind == exp_TERM) && (term->val.expression->val.term->kind == term_NUM)){
                term->kind = term_NUM;
                term->val.num = abs(term->val.expression->val.term->val.num); // Should probably use a temp value instead of such a long value
            }
            break;

        default:
            break;

    }

    return term;

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