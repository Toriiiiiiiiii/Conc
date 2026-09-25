#include "runtime.h"

runtime_t *create_runtime(operation_t *program)
{
	runtime_t *rt = malloc(sizeof(runtime_t));

	rt->stack = malloc(STACK_SIZE * sizeof(uint64_t));
	rt->heap = malloc(HEAP_SIZE * sizeof(uint64_t));

	rt->stack_ptr = 0;
	rt->root = program;

	return rt;
}

void destroy_runtime(runtime_t *rt)
{
	free(rt->stack);
	free(rt->heap);
	free(rt);
}


void _run_node(runtime_t *r, operation_t opr)
{
	if(opr.type == OPR_CODE) {
		for(int i = 0; i < opr.children.nmem; ++i)
		{
			_run_node(r, da_peek(opr.children, i, operation_t));
		}
	} else if(opr.type == OPR_PUSH_INT)
	{
		uint64_t v = atol(opr.value);

		r->stack[r->stack_ptr] = v;
		r->stack_ptr++;
	} else if(opr.type == OPR_BINARY)
	{
		uint64_t b = r->stack[r->stack_ptr - 1];
		uint64_t a = r->stack[r->stack_ptr - 2];
		r->stack_ptr -= 1;

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
		}
	} else if(opr.type == OPR_PRINT)
	{
		printf("%lu\n", r->stack[r->stack_ptr - 1]);
		r->stack_ptr--;
	}
}

void run_program(runtime_t *r)
{
	_run_node(r, *(r->root));
}
