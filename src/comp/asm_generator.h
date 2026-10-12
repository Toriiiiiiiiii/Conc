#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../common/parser.h"

struct _variable_t
{
	char *identifier;
	uint64_t stack_offset;
};

typedef struct _variable_t variable_t;

struct _asmgen_t
{
	operation_t *root;
	dynamic_array_t fns;

	size_t ifs;
	size_t cmps;

	char *outp_path;
	FILE *file;
};

typedef struct _asmgen_t asmgen_t;

asmgen_t *create_generator(operation_t *program, char *path);
void destroy_generator(asmgen_t *g);

void translate(asmgen_t *g);
