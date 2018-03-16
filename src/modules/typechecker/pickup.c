#include <stdio.h>
#include "debug.h"
#include "pickup.h"
#include "tree.h"
#include "symbol.h"
#include "error.h"



void pickup_body(body *body){
#ifdef debugflag
    printf("Picking up body\n");
#endif
    pickup_dlist(body->d_list);

}

void pickup_function(function *function){
#ifdef debugflag
    printf("Picking up function\n");
#endif
    pickup_head(function->head);
    pickup_body(function->body);
    function->stype = function->head->stype;
}

void pickup_head(head *head){
#ifdef debugflag
    printf("Picking up head\n");
#endif
    pickup_pdl(head->list);
    pickup_type(head->type);
#ifdef debugflag
    printf("Picked up Type in Head, Head type: %d, Symbol type: %d\n", head->type->kind, head->stype->type);
#endif
    head->stype->val.func_type.ret_type = head->type;
#ifdef debugflag
    printf("Assigned ret type\n");
#endif
}

void pickup_pdl(par_decl_list *pdl){
#ifdef debugflag
    printf("Picking up pdl\n");
#endif
    if (pdl->kind == pdl_LIST){
        pickup_vdl(pdl->list);
    }

}

void pickup_vdl(var_decl_list *vdl){
#ifdef debugflag
    printf("Picking up vdl\n");
#endif
    pickup_vtype(vdl->vartype);
    if (vdl->kind == vdl_LIST){
        pickup_vdl(vdl->list);
    }
    
}

void pickup_vtype(var_type *vtype){
#ifdef debugflag
    printf("Picking up vtype, id: %s\n", vtype->id);
#endif
    pickup_type(vtype->type);
#ifdef debugflag
    printf("Picked up Type in VType\n");
#endif
    vtype->symbol->stype = vtype->type->stype;

}

void pickup_dlist(decl_list *dlist){
#ifdef debugflag
    printf("Picking up dlist\n");
#endif
    if (dlist->kind == dl_LIST){
        pickup_declaration(dlist->decl);
        pickup_dlist(dlist->list);
    }
}

void pickup_declaration(declaration *decl){
#ifdef debugflag
    printf("Picking up declaration\n");
#endif

    switch (decl->kind){

        case (decl_FUNC):
            pickup_function(decl->val.function);
            break;

        case (decl_TYPE):
            pickup_type(decl->val.type.type);
            break;

        case (decl_VAR):
            pickup_vdl(decl->val.list);
            break;
    }

}

void pickup_type(type *type){
#ifdef debugflag
    printf("Picking up type, kind: %d\n", type->kind);
#endif

    SYMBOL *s;
    switch (type->kind){

        case (type_ARRAY):
            pickup_type(type->val.type);
            type->stype->val.array_type = type->val.type;
            break;

        case (type_BOOl):
            break;

        case (type_INT):
            break;

        case (type_RECORD):
            pickup_vdl(type->val.list);
            type->stype->val.record_type = type->val.list;
            break;

        case (type_ID):
#ifdef debugflag
            printf("ID we are looking for: %s, in table: %p\n", type->val.id, type->table);
#endif
            s = get_symbol(type->table, type->val.id);
            if (s == NULL || s->stype->type != symbol_ID){
                if (s == NULL){
                    printf("Symbol is NULL\n");
                }
                if (s->stype->type != symbol_ID){
                    printf("Symbol is not ID, it is of type: %d\n", s->stype->type);
                }
                print_error("Identifier error", 0, type->lineno);
            }
            struct type *temp;
            temp = resolve_recursive_type(s->stype->val.id_type);
#ifdef debugflag
            printf("After recursive check\n");
#endif
            type->stype = temp->stype;
#ifdef debugflag
            printf("After assignment\n");
#endif
    }
#ifdef debugflag
    printf("After switch\n");
#endif

}

type *resolve_recursive_type(type *type){
#ifdef debugflag
    printf("Resolving recursive conflict\n");
#endif
    struct type *temp;
    temp = type;
    if (type->recursive_type == 1){
        print_error("Recursive type definition", 0, type->lineno);
    }
    type->recursive_type = 1;
#ifdef debugflag
    printf("Type kind: %d\n", type->kind);
#endif
    if (type->kind == type_ID){
        printf("Checking symbol table for symbol\n");
        SYMBOL *s;
        s = get_symbol(type->table, type->val.id);
        if (s == NULL || s->stype->type != symbol_ID){
            print_error("Undefined identifier", 0, type->lineno);
        }
        temp = resolve_recursive_type(s->stype->val.id_type);
    }
#ifdef debugflag
    printf("Checked recursively\n");
#endif
    type->recursive_type = 0;
    return temp;
}