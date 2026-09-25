#pragma once
#include <stdlib.h>
#include <string.h>

#include "da.h"

enum _token_type_t
{
	TOK_NULL,
	TOK_EOF,
	TOK_INT,
	TOK_OPERATOR,
	TOK_KEYWORD,
	TOK_PRINT,
};

typedef enum _token_type_t token_type_t;

struct _token_t
{
	size_t file;
	size_t line, col;

	size_t len;
	token_type_t type;

	char *value;
};

typedef struct _token_t token_t;


struct _lexer_t
{
	dynamic_array_t tokens;

	size_t file;
	size_t offset;
	size_t len;

	size_t line, col;

	char *buffer;
};

typedef struct _lexer_t lexer_t;

lexer_t *create_lexer(size_t file, char *buffer, size_t len);
void destroy_lexer(lexer_t *lex);

int lex_token(lexer_t *lex);
