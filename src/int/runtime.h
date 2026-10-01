#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../common/parser.h"

#define STACK_SIZE 65536
#define HEAP_SIZE  16777216

struct _variable_t
{
	char *identifier;
	uint64_t value;
};

typedef struct _variable_t variable_t;

struct _scope_t
{
	struct _scope_t *parent;
	dynamic_array_t vars;
};

typedef struct _scope_t scope_t;

struct _runtime_t
{
	uint64_t *stack;
	uint64_t *heap;

	size_t stack_ptr;

	operation_t *root;
	dynamic_array_t fns;
	scope_t *root_scope;
};

typedef struct _runtime_t runtime_t;

runtime_t *create_runtime(operation_t *program);
void destroy_runtime(runtime_t *rt);

void run_program(runtime_t *r);
