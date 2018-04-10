#include <stdio.h>
#include "debug.h"
#include "tree.h"
#include "check.h"
#include "memory.h"
#include "error.h"
#include "symbol.h"
#include "pickup.h"


void check_body(body *body){

    check_dlist(body->d_list);
    check_slist(body->s_list);

}

void check_function(function *function){

    check_body(function->body);

}

void check_dlist(decl_list *dlist){

    if (dlist->kind == dl_LIST){
        check_dlist(dlist->list);
        check_decl(dlist->decl);
    }

}

void check_decl(declaration *decl){

    if (decl->kind == decl_FUNC){
        check_function(decl->val.function);
    }

}

void check_slist(statement_list *slist){

    check_stmt(slist->statement);
    if (slist->kind == sl_LIST){
        check_slist(slist->list);
    }

}

void check_stmt(statement *stmt){

    
    switch(stmt->kind){

        case (statement_RETURN):
            check_exp(stmt->val.ret);
            if (stmt->function->stype->val.func_type.ret_type->stype->type != stmt->val.ret->stype->type){
                print_error("Wrong return type", 0, stmt->lineno);
            }
            break;

        case (statement_WRITE):
            check_exp(stmt->val.wrt);
            if ((stmt->val.wrt->stype->type != symbol_INT) && (stmt->val.wrt->stype->type != symbol_BOOL) && (stmt->val.wrt->stype->type != symbol_ID)){
               print_error("Wrong write type", 0, stmt->lineno);
            }
            break;

        case (statement_ALLOCATE):
            check_var(stmt->val.allocate.variable);
#if debugflag > 3
            printf("Allocating type: %d\n", stmt->val.allocate.variable->stype->type);
#endif
            if ( (stmt->val.allocate.variable->stype->type != symbol_ARRAY) && (stmt->val.allocate.variable->stype->type != symbol_RECORD)){
                print_error("Wrong allocate type", 0, stmt->lineno);
            }
            break;

        case (statement_ALLOCATE_LENGTH):
            check_var(stmt->val.allocate.variable);
            check_exp(stmt->val.allocate.length);
            if ( (stmt->val.allocate.variable->stype->type != symbol_ARRAY) && (stmt->val.allocate.variable->stype->type != symbol_RECORD)){
                print_error("Wrong allocate type", 0, stmt->lineno);
            }
            if (stmt->val.allocate.length->stype->type != symbol_INT){
                print_error("Allocate length must be integer", 0, stmt->lineno);
            }
            break;

        case (statement_ASSIGNMENT):
            check_var(stmt->val.assignment.variable);
            check_exp(stmt->val.assignment.expression);
            if (stmt->val.assignment.expression->stype->type == symbol_NULL){
                if ( (stmt->val.assignment.variable->stype->type != symbol_ARRAY) && (stmt->val.assignment.variable->stype->type != symbol_RECORD)){
                    print_error("Can only assign array and record to NULL", 0 , stmt->lineno);
                }
                break;
            }
            if (!compare_stype(stmt->val.assignment.variable->stype, stmt->val.assignment.expression->stype)){
                print_error("Incompatible type assignment", 0, stmt->lineno);
            }
            break;

        case (statement_IF):
            check_exp(stmt->val.ifthen.expression);
            check_stmt(stmt->val.ifthen.statement1);
            if (stmt->val.ifthen.expression->stype->type != symbol_BOOL){
                print_error("If condition is not a boolean", 0, stmt->lineno);
            }
            break;

        case (statement_IF_ELSE):
            check_exp(stmt->val.ifthen.expression);
            check_stmt(stmt->val.ifthen.statement1);
            check_stmt(stmt->val.ifthen.statement2);
            if (stmt->val.ifthen.expression->stype->type != symbol_BOOL){
                print_error("If condition is not a boolean", 0, stmt->lineno);
            }
            break;
        
        case (statement_WHILE):
            check_exp(stmt->val.loop.expression);
            check_stmt(stmt->val.loop.statement);
            break;

        case (statement_LIST):
            check_slist(stmt->val.list);
            break;

    }

}

void check_var(variable *var){
#if debugflag > 2
    printf("Checking variable, kind: %d\n", var->kind);
#endif

    SYMBOL *s;
    switch (var->kind){

        case (var_ID):
            s = get_symbol(var->table, var  ->id);
            if (s == NULL){
                print_error("Symbol not defined", 0, var->lineno);
            }
            var->stype = s->stype;
            break;

        case (var_EXP):
            check_var(var->val.exp.var);
            check_exp(var->val.exp.exp);
            if (var->val.exp.exp->stype->type != symbol_INT){
                print_error("Expression in [] not an integer", 0, var->lineno);
            }
            if (var->val.exp.var->stype->type != symbol_ARRAY){
                print_error("Variable is not an array", 0, var->lineno);
            }
            var->stype = var->val.exp.var->stype->val.array_type->stype;
            break;

        case (var_RECORD):
            check_var(var->val.record.var);
            if (var->val.record.var->stype->type != symbol_RECORD){
                print_error("Variable is not an record", 0, var->lineno);
            }
            s = get_symbol(var->val.record.var->stype->val.record_type->table, var->val.record.id);
            if (s == NULL){
                print_error("Record entry does not exist", 0, var->lineno);
            }
            var->stype = s->stype;
            break;
    }

}

void check_exp(expression *exp){
#if debugflag > 2
    printf("Checking expression, kind %d\n", exp->kind);
#endif
    symbol_type *st;

    switch(exp->kind){

        case (exp_PLUS):        // Subject to change, could be made to work with strings, f.x. "Hi"+2 could be: "Hi2"
        case (exp_MIN):
        case (exp_MULT):        // Subject to change, could be made to work with strings, f.x. "Hi"*2 could be: "HiHi"
        case (exp_DIV):
#if debugflag > 3
            printf("Checking left expression, Arithmetic\n");
#endif
            check_exp(exp->val.ops.left);
#if debugflag > 3
            printf("Checking right expression, Arithmetic\n");
#endif
            check_exp(exp->val.ops.right);
            if (exp->val.ops.left->stype->type == symbol_INT && exp->val.ops.right->stype->type == symbol_INT){

                st = NEW(symbol_type);
                st->type = symbol_INT;
                exp->stype= st;

            } else {
                print_error("Operators in arithmetic expression are not integers", 0, exp->lineno);
            }
            break;
        
        case (exp_EQ):
        case (exp_NEQ):
#if debugflag > 2
            printf("Checking left expression, EQ/NEQ\n");
#endif
            check_exp(exp->val.ops.left);
#if debugflag > 2
            printf("Checking right expression, EQ/NEQ\n");
#endif
            check_exp(exp->val.ops.right);

            // Should check stype->type->val.func_type.ret_type if comparing a function
            if ((exp->val.ops.left->stype->type == symbol_RECORD) && (exp->val.ops.right->stype->type == symbol_NULL)){
#if debugflag > 3
                printf("Left type is record, and right type is NULL\n");
#endif
                st = NEW(symbol_type);
                st->type = symbol_BOOL;
                exp->stype = st;
                break;
            }

            if ((exp->val.ops.left->stype->type == symbol_NULL) && (exp->val.ops.right->stype->type == symbol_RECORD)){
#if debugflag > 3
                printf("Left type is NULL, and right type is record\n");
#endif
                st = NEW(symbol_type);
                st->type = symbol_BOOL;
                exp->stype = st;
                break;
            }
#if debugflag > 3
            printf("Left sides type: %d, Right sides type: %d\n", exp->val.ops.left->stype->type, exp->val.ops.right->stype->type);
#endif
            if (exp->val.ops.left->stype->type == exp->val.ops.right->stype->type){
#if debugflag > 3
                printf("Checked if type is the same\n");
#endif
                st = NEW(symbol_type);
                st->type = symbol_BOOL;
                exp->stype = st;

            } else {
                print_error("Operators in EQ or NEQ not the same", 0, exp->lineno);
            }
            break;
        
        case (exp_GEQ):
        case (exp_LEQ):
        case (exp_LT):
        case (exp_GT):
#if debugflag > 2
            printf("Checking left expression, GEQ/LEQ/LT/GT\n");
#endif
            check_exp(exp->val.ops.left);
#if debugflag > 2
            printf("Checking right expression, GEQ/LEQ/LT/GT\n");
#endif
            check_exp(exp->val.ops.right);
            if (exp->val.ops.left->stype->type == symbol_INT && exp->val.ops.right->stype->type == symbol_INT){

                st = NEW(symbol_type);
                st->type = symbol_BOOL;
                exp->stype = st;
            
            } else {
                print_error("Operators used in GEQ, LEQ, LT or GT not integers", 0, exp->lineno);
            }
            break;

        case (exp_AND):
        case (exp_OR):
#if debugflag > 2
            printf("Checking left expression, AND/OR\n");
#endif
            check_exp(exp->val.ops.left);
#if debugflag > 2
            printf("Checking right expression, AND/OR\n");
#endif
            check_exp(exp->val.ops.right);

            if (exp->val.ops.left->stype->type == symbol_BOOL && exp->val.ops.right->stype->type == symbol_BOOL){

                st = NEW(symbol_type);
                st->type = symbol_BOOL;
                exp->stype = st;

            } else {
                print_error("Operators used in AND or OR not boolean", 0, exp->lineno);
            }
            break;

        case (exp_TERM):
            check_term(exp->val.term);
            exp->stype = exp->val.term->stype;
            break;

    }

}

void check_term(term *term){
#if debugflag > 2
    printf("Checking term, kind: %d\n", term->kind);
#endif

    SYMBOL *s;
    symbol_type *st;

    switch(term->kind){

        case (term_VAR):
            check_var(term->val.variable);
            term->stype = term->val.variable->stype;
#if debugflag > 3
            printf("Term type: %d\n", term->stype->type);
#endif
            break;

        case (term_LIST):
            check_alist(term->val.list.list);
            s = get_symbol(term->table, term->val.list.id);
            if (s == NULL || s->stype->type != symbol_FUNCTION){
                print_error("Reference to function that does not exists", 0, term->lineno);
            }
            check_function_args(s->stype->val.func_type.pdl, term->val.list.list);
            term->stype = s->stype->val.func_type.ret_type->stype;
            break;

        case (term_PAR):
            check_exp(term->val.expression);
            term->stype->type = term->val.expression->stype->type;
            break;

        case (term_NOT):
            check_term(term->val.term_not);
            if (term->val.term_not->stype->type != symbol_BOOL){
                print_error("Cannot negate non-boolean", 0, term->lineno);
                
            } 
            term->stype->type = term->val.term_not->stype->type;
            break;

        case (term_ABS):
            check_exp(term->val.expression);
#if debugflag > 3
            printf("Type of expression: %d\n", term->val.expression->stype->type);
#endif
            if ((term->val.expression->stype->type != symbol_INT) && (term->val.expression->stype->type != symbol_ARRAY)){
                print_error("Absolute value must be used on integer or array", 0 , term->lineno);
            }
            st = NEW(symbol_type);
            st->type = symbol_INT;
            term->stype = st;
            break;

        case (term_NUM):
            st = NEW(symbol_type);
            st->type = symbol_INT;
            term->stype = st;
            break;

        case (term_TRUE):
        case (term_FALSE):
            st = NEW(symbol_type);
            st->type = symbol_BOOL;
            term->stype = st;
            break;

        case (term_NULL):
            st = NEW(symbol_type);
            st->type = symbol_NULL;
            term->stype = st;
            break;
            
    }

}

void check_alist(act_list *alist){

    if (alist->kind == al_LIST){
        check_elist(alist->list);
    }

}


void check_elist(exp_list *elist){
    check_exp(elist->expression);
    if (elist->kind == el_LIST){
        check_elist(elist->list);
    }


}

int check_function_args(par_decl_list *pdl, act_list *alist){
    
    struct exp_list *elist;
    struct var_decl_list *vdl;
    struct type *temp;
    struct symbol_type *st1;
    struct symbol_type *st2;

    if (pdl->kind == pdl_EMPTY){
#if debugflag > 3
        printf("PDL is empty\n");
#endif
        if (alist->kind == al_EMPTY){
            return 1;
        } else {
            print_error("Too many function arguments", 0, alist->lineno);
        }
    } else {
        if (alist->kind == al_EMPTY){
            print_error("Too few function arguments", 0, alist->lineno);
        }
    }

    elist = alist->list;
    vdl = pdl->list;

    while ((vdl != NULL) && (elist != NULL)){
        //In case of recursive definition
        temp = resolve_recursive_type(vdl->vartype->type);

        st1 = temp->stype;
        st2 = elist->expression->stype;
#if debugflag > 2
        printf("ST1s type: %d, ST2s type: %d\n", st1->type, st2->type);
#endif

        if (!compare_stype(st1, st2)){
            print_error("Function argument type mismatch", 0, alist->lineno);
        }
        vdl = vdl->list;
        elist = elist->list;
    }
    
    if ((vdl == NULL) && (elist == NULL)){
        return 1;
    } 
    if (vdl == NULL){
        print_error("Too many function arguments", 0, alist->lineno);
    }
    if (elist == NULL){
        print_error("Too few function arguments", 0, alist->lineno);
    }

    return 0;

}

int compare_stype(symbol_type *stype1, symbol_type *stype2){

    if (stype1 == NULL || stype2 == NULL){
#if debugflag > 3
        printf("Both NULL, return 0\n");
#endif
        return 0;
    }

    if (stype1->type == stype2->type){
#if debugflag > 3
        printf("Both equal, return 1\n");
#endif
        return 1;
    }

    if (stype1->type == symbol_ARRAY && stype2->type == symbol_ARRAY){
        return compare_stype(stype1->val.array_type->stype, stype2->val.array_type->stype);
    }

    if (stype1->type == symbol_RECORD && stype2->type == symbol_RECORD){
        return compare_record(stype1, stype2);
    }
#if debugflag > 3
    printf("Not equal, return 0\n");
#endif

    return 0;

}

int compare_record(symbol_type *stype1, symbol_type *stype2){

    var_decl_list *vdl1;
    var_decl_list *vdl2;

    symbol_table *table1;
    symbol_table *table2;

    SYMBOL *s1;
    SYMBOL *s2;

    vdl1 = stype1->val.record_type;
    vdl2 = stype2->val.record_type;

    table1 = stype1->val.record_type->table;
    table2 = stype2->val.record_type->table;

    while ((vdl1 != NULL) && (vdl2 != NULL)){

        s1 = get_symbol(table1, vdl1->vartype->id);
        s2 = get_symbol(table2, vdl2->vartype->id);

        if (s1 == NULL || s2 == NULL){
            return 0;
        }

        if (!compare_stype(s1->stype, s2->stype)){
            return 0;
        }
        vdl1 = vdl1->list;
        vdl2 = vdl2->list;
    }

    if ((vdl1 == NULL) && (vdl2 == NULL)){
        return 1;
    }

    return 0;

}