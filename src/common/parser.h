#pragma once
#include "lexer.h"
#include <stdio.h>

enum _operation_type_t
{
	OPR_NULL,
	OPR_CODE,
	OPR_PUSH_INT,
	OPR_BINARY,
	OPR_UNARY,
	OPR_PRINT,
};

typedef enum _operation_type_t operation_type_t;

struct _operation_t
{
	size_t file;
	size_t line, col;

	operation_type_t type;
	size_t len;
	char *value;

	struct _operation_t *parent;
	dynamic_array_t children;
};

typedef struct _operation_t operation_t;

struct _parser_t
{
	dynamic_array_t tokens;
	operation_t *root;

	size_t offset;
};

typedef struct _parser_t parser_t;

parser_t *create_parser(dynamic_array_t tokens);
void destroy_parser(parser_t *parser);

int parse_opr(parser_t *parser);

static inline void print_opr(operation_t opr)
{
	if(opr.children.nmem) {
		printf("%d : %s  {  ", opr.type, opr.value);

		for(int i = 0; i < opr.children.nmem; ++i)
		{
			print_opr(da_peek(opr.children, i, operation_t));
			printf("  ");
		}

		printf("}");
	} else {
		printf("%d : %s", opr.type, opr.value);
	}
}
