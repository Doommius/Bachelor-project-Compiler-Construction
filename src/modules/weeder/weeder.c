#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "weeder.h"


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

int weeder(body *program){
    //TODO Skal nok vaere noget mere her
    weed_body(program);

}

body *weed_body(body *body){
    body->d_list = weed_dlist(body->d_list);
    body->s_list = weed_slist(body->s_list);
    return body;
}

function *weed_function(function *func){

}

head *weed_head(head *head){

}

tail *weed_tail(tail *tail){

}

type *weed_type(type *type){

}

par_decl_list *weed_pdl(par_decl_list *pdl){

}

var_decl_list *weed_vdl(var_decl_list *vdl){

}

var_type *weed_vtype(var_type *vtype){

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

}

statement_list *weed_slist(statement_list *slist){

}

statement *weed_stmt(statement *stmt){

}

variable *weed_variable(variable *variable){

}

expression *weed_expression(expression *expression){

}

term *weed_term(term *term){

}

act_list *weed_alist(act_list *alist){

}

exp_list *weed_elist(exp_list *elist){

}