#include <stdio.h>
#include "debug.h"
#include "pickup.h"
#include "tree.h"
#include "symbol.h"
#include "error.h"

void pickup_body(body *body) {
	pickup_dlist(body->d_list);
}

void pickup_function(function *function) {
	pickup_head(function->head);
	pickup_body(function->body);
	function->stype = function->head->stype;
}

void pickup_head(head *head) {
	pickup_pdl(head->list);
	pickup_type(head->type);

	head->stype->val.func_type.ret_type = head->type;
}

void pickup_pdl(par_decl_list *pdl) {
	if (pdl->kind == pdl_LIST) {
		pickup_vdl(pdl->list);
	}
}

void pickup_vdl(var_decl_list *vdl) {
	pickup_vtype(vdl->vartype);
	if (vdl->kind == vdl_LIST) {
		pickup_vdl(vdl->list);
	}
}

void pickup_vtype(var_type *vtype) {
	pickup_type(vtype->type);
	vtype->symbol->stype = vtype->type->stype;
}

void pickup_dlist(decl_list *dlist) {
	if (dlist->kind == dl_LIST) {
		pickup_declaration(dlist->decl);
		pickup_dlist(dlist->list);
	}
}

void pickup_declaration(declaration *decl) {

	switch (decl->kind) {

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

void pickup_type(type *type) {

	SYMBOL *s;
	switch (type->kind) {

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

		s = get_symbol(type->table, type->val.id);
		if (s == NULL || s->stype->type != symbol_ID) {
			if (s == NULL) {
				printf("Symbol is NULL\n");
			}
			if (s->stype->type != symbol_ID) {
				printf("Symbol is not ID, it is of type: %d\n", s->stype->type);
			}
			print_error("Identifier error", 0, type->lineno);
		}
		struct type *temp;
		temp = resolve_recursive_type(s->stype->val.id_type);

		type->stype = temp->stype;
	}
}

type *resolve_recursive_type(type *type) {

	struct type *temp;
	temp = type;
	if (type->recursive_type == 1) {
		print_error("Recursive type definition", 0, type->lineno);
	}
	type->recursive_type = 1;

	if (type->kind == type_ID) {
		SYMBOL *s;
		s = get_symbol(type->table, type->val.id);
		if (s == NULL || s->stype->type != symbol_ID) {
			print_error("Undefined identifier", 0, type->lineno);
		}
		temp = resolve_recursive_type(s->stype->val.id_type);
	}

	type->recursive_type = 0;
	return temp;
}