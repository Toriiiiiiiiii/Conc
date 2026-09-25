#include "lexer.h"

lexer_t *create_lexer(size_t file, char *buffer, size_t len)
{
	lexer_t *lex = (lexer_t *)malloc(sizeof(lexer_t));
	if(!lex) return NULL;

	lex->tokens = da_create(token_t);
	lex->file = file;
	lex->offset = 0;
	lex->len = len;

	lex->line = lex->col = 1;
	lex->buffer = buffer;

	return lex;
}

void destroy_lexer(lexer_t *lex)
{
	if(lex->tokens.arrbase) {
		for(int i = 0; i < lex->tokens.nmem; ++i)
		{
			free(da_peek(lex->tokens, i, token_t).value);
		}
	}
	da_destroy(lex->tokens);
	free(lex);
}

int contains(char *s, char c)
{
	for(int i = 0; i < strlen(s); ++i)
		if(s[i] == c) return 1;

	return 0;
}

char lex_consume(lexer_t *lex)
{
	char c = lex->buffer[lex->offset];
	lex->offset++;

	lex->col++;
	if(c == '\n') {
		lex->col = 1;
		lex->line++;
	}

	return c;
}

void lex_number(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_INT;
	tok.value = malloc(25);

	int i = 0;
	char c = lex_consume(lex);
	while(lex->offset < lex->len && i < 24 && contains("0124356789", c))
	{
		tok.value[i] = c;
		c = lex_consume(lex);
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	da_push(lex->tokens, tok, token_t);
}

void lex_operation(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_OPERATOR;
	tok.value = malloc(25);

	int i = 0;
	char c = lex_consume(lex);
	while(lex->offset < lex->len && i < 24 && contains("+-*/&^><!~", c))
	{
		tok.value[i] = c;
		c = lex_consume(lex);
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	da_push(lex->tokens, tok, token_t);
}

int lex_token(lexer_t *lex)
{
	char c = 0;

	while(!contains(" \t\n", c))
	{
		c = lex->buffer[lex->offset];

		if(c == 0) {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_EOF;

			da_push(lex->tokens, tok, token_t);
			return 1;
		}

		if(contains("0123456789", c)) {
			lex_number(lex);
			return 0;
		}
		else if(contains("+-*/&^><!~", c)) {
			lex_operation(lex);
			return 0;
		}

		else if(c == '.') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_PRINT;

			da_push(lex->tokens, tok, token_t);
		}

		lex->offset++;
	}

	return 0;
}
