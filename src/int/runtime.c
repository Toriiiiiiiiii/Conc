#include "runtime.h"

scope_t *create_scope(scope_t *parent)
{
	scope_t *s = malloc(sizeof(scope_t));
	s->parent = parent;
	s->vars = da_create(variable_t*);
	return s;
}

variable_t *get_var(scope_t *scope, char *identifier)
{
	for(int i = 0; i < scope->vars.nmem; ++i) {
		variable_t *var = da_peek(scope->vars, i, variable_t*);
		if(strcmp(var->identifier, identifier) == 0) return var;
	}

	if(!scope->parent) return NULL;
	return get_var(scope->parent, identifier);
}

uint64_t stk_pop(runtime_t *r)
{
	if(r->stack_ptr == 0) {
		printf("[ERROR] Stack underflow.\n");
		exit(1);
	}

	r->stack_ptr--;
	return r->stack[r->stack_ptr];
}

void set_var(scope_t *scope, char *identifier, uint64_t value)
{
	variable_t *var = get_var(scope, identifier);
	var->value = value;
}

runtime_t *create_runtime(operation_t *program)
{
	runtime_t *rt = malloc(sizeof(runtime_t));

	rt->stack = malloc(STACK_SIZE * sizeof(uint64_t));
	rt->heap = malloc(HEAP_SIZE * sizeof(uint64_t));

	rt->stack_ptr = 0;
	rt->root = program;

	rt->fns = da_create(operation_t);

	return rt;
}

void destroy_runtime(runtime_t *rt)
{
	free(rt->stack);
	free(rt->heap);
	free(rt);
}


void _run_node(runtime_t *r, operation_t opr, scope_t *scope)
{
	if(opr.type == OPR_CODE) {
		for(int i = 0; i < opr.children.nmem; ++i)
		{
			_run_node(r, da_peek(opr.children, i, operation_t), scope);
		}
	} else if(opr.type == OPR_PUSH_INT)
	{
		uint64_t v = atol(opr.value);

		r->stack[r->stack_ptr] = v;
		r->stack_ptr++;
	} else if(opr.type == OPR_BINARY)
	{
		uint64_t b = stk_pop(r);
		uint64_t a = stk_pop(r);
		r->stack_ptr += 1;

		switch(opr.value[0]) {
			case '+':
				r->stack[r->stack_ptr - 1] = a + b;
				break;
			case '-':
				r->stack[r->stack_ptr - 1] = a - b;
				break;
			case '*':
				r->stack[r->stack_ptr - 1] = a * b;
				break;
			case '/':
				r->stack[r->stack_ptr - 1] = a / b;
				break;
			case '%':
				r->stack[r->stack_ptr - 1] = a % b;
				break;
			case '=':
				r->stack[r->stack_ptr - 1] = a == b;
				break;
			case '<':
				r->stack[r->stack_ptr - 1] = a < b;
				break;
			case '>':
				r->stack[r->stack_ptr - 1] = a > b;
				break;
		}
	} else if(opr.type == OPR_UNARY) {
		uint64_t a = stk_pop(r);
		r->stack_ptr += 2;

		switch(opr.value[0]) {
			case ':':
				r->stack[r->stack_ptr - 1] = a;
		}
	} else if(opr.type == OPR_PRINT)
	{
		printf("%lu\n", stk_pop(r));
	} else if(opr.type == OPR_KEYWORD)
	{
		variable_t *var = get_var(scope, opr.value);
		if(var)
		{
			r->stack[r->stack_ptr] = var->value;
			r->stack_ptr++;
			return;
		}

		operation_t fn = {0};

		for(int i = 0; i < r->fns.nmem; ++i)
		{
			fn = da_peek(r->fns, i, operation_t);
			if(strcmp(opr.value, fn.value) == 0) break;

			fn = (operation_t){0};
		}

		if(fn.type != OPR_PROC) {
			printf("[ERROR] Unknown reference to '%s'\n", opr.value);
			return;
		}

		scope_t *new_scope = create_scope(r->root_scope);
		_run_node(r, da_peek(fn.children, 0, operation_t), new_scope);
	} else if(opr.type == OPR_PROC)
	{
		da_push(r->fns, opr, operation_t);
	} else if(opr.type == OPR_ASSIGN)
	{
		r->stack_ptr--;

		variable_t *var = get_var(scope, opr.value);
		if(var) {
			var->value = r->stack[r->stack_ptr];
			return;
		}

		variable_t *v = malloc(sizeof(variable_t));
		v->identifier = opr.value;
		v->value = r->stack[r->stack_ptr];
		da_push(scope->vars, v, variable_t*);
	} else if(opr.type == OPR_IF)
	{
		_run_node(r, da_peek(opr.children, 0, operation_t), scope);
		r->stack_ptr--;

		scope_t *new_scope = create_scope(scope);
		if(r->stack[r->stack_ptr]) _run_node(r, da_peek(opr.children, 1, operation_t), new_scope);
		else if(opr.children.nmem >= 3) _run_node(r, da_peek(opr.children, 2, operation_t), new_scope);
	}
}

void run_program(runtime_t *r)
{
	r->root_scope = create_scope(NULL);
	_run_node(r, *(r->root), r->root_scope);
}
