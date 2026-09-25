#include "parser.h"

parser_t *create_parser(dynamic_array_t tokens)
{
	if(!tokens.arrbase) return NULL;

	parser_t *par = (parser_t*)malloc(sizeof(parser_t));
	if(!par) return NULL;

	par->tokens = tokens;
	par->root = (operation_t*)malloc(sizeof(operation_t));

		par->root->file     = 0;
		par->root->line     = 0;
		par->root->col      = 0;
		par->root->type     = OPR_CODE;
		par->root->len      = 0;
		par->root->value    = NULL;
		par->root->parent   = NULL;
		par->root->children = da_create(operation_t);

	par->offset = 0;

	return par;
}

void destroy_parser(parser_t *par)
{
	free(par->root);
	free(par);
}

token_t _peek(parser_t *par)
{
	return da_peek(par->tokens, par->offset, token_t);
}

token_t _consume(parser_t *par)
{
	return da_peek(par->tokens, par->offset++, token_t);
}

int _parse(parser_t *par, operation_t *parent)
{
	token_t t = _consume(par);

	operation_t opr = {0};

	switch(t.type) {
		case TOK_NULL: break;
		case TOK_EOF: return 1;
		case TOK_INT:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_PUSH_INT,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		case TOK_OPERATOR:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_BINARY,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		case TOK_PRINT:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_PRINT,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		default:
			break;
	}

	da_push(parent->children, opr, operation_t);
	return 0;
}

int parse_opr(parser_t *parser)
{
	return _parse(parser, parser->root);
}
