#include "lexer.h"
#include <ctype.h>

/******************************************************************
 *
 * Lexer Helper Functions
 *
 *****************************************************************/

/* create_lexer : Instantiate a lexer structure.
 *   Parameters:
 *     * file : Index of the file (see: file.h)
 *     * buffer : Input string to analyse
 *     * len : Length of the input string
 *
 *   Returns:
 *     * Lexer structure with valid fields ready for analysis.
 */
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


/* destroy_lexer : Frees up a lexer structure.
 *    Parameters:
 *      * lex : Lexer object to destroy
 */
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


/* contains : Determins if a string contains a certain character.
 *    Parameters:
 *      * s : The string to search
 *      * c : The character to find
 *
 *    Returns:
 *      * 1 : If `s` contains `c`
 *      * 0 : If `s` does not contain `c`
 *
 *    Notes:
 *      * This is just a very basic linear search algorithm.
 */
int contains(char *s, char c)
{
	for(int i = 0; i < strlen(s); ++i)
		if(s[i] == c) return 1;

	return 0;
}


/* lex_consume : Fetches the next character from a lexer's input and advances.
 *    Parameters:
 *      * lex : The lexer to advance.o
 *
 *    Returns:
 *      * The next character from the input string
 *
 *    Notes:
 *      * This function handles newlines, updating the `col` and `line` fields
 *        of the lexer.
 */
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


/******************************************************************
 *
 * Lexical Analysis Functions
 *
 *****************************************************************/

/* All of the functions within this section of the analysis perform
 * very similar algorithms. They simply extract a string of characters
 * from the source based on different conditions.
 *
 * Functions:
 *   * lex_number : Extracts unsigned integers
 *   * lex_keyword : Extracts c-style identifiers
 *   * lex_operation : Extracts binary operators
 *   * lex_unary : Extracts unary operators
 *   * lex_string : Extracts a bounded c-style string.
 */

void lex_number(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_INT;
	tok.value = malloc(25);

	int i = 0;
	char c = lex->buffer[lex->offset];
	while(lex->offset < lex->len && i < 24 && contains("0124356789", c))
	{
		lex_consume(lex);
		tok.value[i] = c;
		c = lex->buffer[lex->offset];
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	da_push(lex->tokens, tok, token_t);
}


void lex_keyword(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_KEYWORD;
	tok.value = malloc(25);

	int i = 0;
	char c = lex->buffer[lex->offset];
	while(lex->offset < lex->len && i < 24 && (isalnum(c) || contains("_.", c)))
	{
		lex_consume(lex);
		tok.value[i] = c;
		c = lex->buffer[lex->offset];
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
	char c = lex->buffer[lex->offset];
	while(lex->offset < lex->len && i < 24 && contains("+-*/&^><%=", c))
	{
		lex_consume(lex);
		tok.value[i] = c;
		c = lex->buffer[lex->offset];
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	da_push(lex->tokens, tok, token_t);
}


void lex_unary(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_UNARY;
	tok.value = malloc(25);

	int i = 0;
	char c = lex->buffer[lex->offset];
	while(lex->offset < lex->len && i < 24 && contains("#:@$!", c))
	{
		lex_consume(lex);
		tok.value[i] = c;
		c = lex->buffer[lex->offset];
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	da_push(lex->tokens, tok, token_t);
}


void lex_string(lexer_t *lex)
{
	token_t tok = {0};
	tok.file = lex->file;
	tok.line = lex->line;
	tok.col = lex->col;

	tok.type = TOK_STRING;
	tok.value = malloc(1024);

	int i = 0;

	char term = lex_consume(lex);

	char c = lex->buffer[lex->offset];
	while(lex->offset < lex->len && i < 1024 && c != term)
	{
		lex_consume(lex);

		if(c == '\\') {
			if(lex_consume(lex) == 'n') c = '\n';
		}

		tok.value[i] = c;
		c = lex->buffer[lex->offset];
		i++;
	}

	tok.value[i] = 0;
	tok.len = strlen(tok.value);
	tok.value = realloc(tok.value, tok.len + 1);
	da_push(lex->tokens, tok, token_t);
	lex_consume(lex);
}


/* lex_token : Master function for lexical analysis
 *    Parameters:
 *      * lex : The lexer data structure to perform
 *              analysis with.
 *
 *    Returns:
 *      * 0 : If analysis is still ongoing
 *      * 1 : If analysis is finished
 *
 *    Notes:
 *      * This function is a mess D:
 *        TODO: Tidy this shit up and use less if-else
 */

int lex_token(lexer_t *lex)
{
	char c = 0;

	while(!contains(" \t\n", c))
	{
		c = lex->buffer[lex->offset];

		if(c == ';') {
			while(c != '\n' && c != '\0') c = lex_consume(lex);
			return 0;
		}

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
		else if(contains("+-*/&^><!%=", c)) {
			lex_operation(lex);
			return 0;
		}
		else if(contains("#:@$!", c)) {
			lex_unary(lex);
			return 0;
		}
		else if(isalpha(c) || c == '_') {
			lex_keyword(lex);
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
		else if(c == '?') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_IF;

			da_push(lex->tokens, tok, token_t);
		}
		else if(c == '~') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_WHILE;

			da_push(lex->tokens, tok, token_t);
		}
		else if(c == ',') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_ELSE;

			da_push(lex->tokens, tok, token_t);
		}
		else if(c == '(') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_LPAREN;

			da_push(lex->tokens, tok, token_t);
		}
		else if(c == ')') {
			token_t tok = {0};

			tok.file = lex->file;
			tok.line = lex->line;
			tok.col = lex->col;
			tok.len = 0;
			tok.value = NULL;
			tok.type = TOK_RPAREN;

			da_push(lex->tokens, tok, token_t);
		}
		else if(c == '"' || c == '\'') {
			lex_string(lex);
			return 0;
		}

		lex->offset++;
	}

	return 0;
}
