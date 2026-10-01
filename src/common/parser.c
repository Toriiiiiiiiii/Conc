#include "parser.h"

int _parse(parser_t *par, operation_t *parent);

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

operation_t _keyword(parser_t *par, operation_t *parent, token_t t)
{
	if(strcmp("do", t.value) == 0) {
		operation_t o = (operation_t){
			t.file, t.line, t.col, OPR_CODE,
			t.len, t.value, parent, da_create(operation_t)
		};

		while(!_parse(par, &o));
		return o;
	}
	else if(strcmp("end", t.value) == 0)  {
		return (operation_t){0};
	}
	else if(strcmp("proc", t.value) == 0) {
		operation_t o = (operation_t){
			t.file, t.line, t.col, OPR_PROC,
			0, NULL, parent, da_create(operation_t)
		};

		token_t name = _consume(par);
		if(name.type != TOK_KEYWORD) {
			printf("[ERROR] : Expected identifier for procedure name.\n");
			return (operation_t){0};
		}

		o.len = name.len;
		o.value = name.value;

		_parse(par, &o);
		return o;
	}

	return (operation_t){
		t.file, t.line, t.col, OPR_KEYWORD,
		t.len, t.value, parent, da_create(operation_t)
	};
}

operation_t _if(parser_t *par, operation_t *parent, token_t t)
{
	operation_t o = (operation_t){
		t.file, t.line, t.col, OPR_IF,
		0, NULL, parent, da_create(operation_t)
	};

	// Parse condition
	_parse(par, &o);

	// Parse body
	_parse(par, &o);

	if(_peek(par).type != TOK_ELSE) return o;
	_consume(par);

	// Parse else
	_parse(par, &o);

	return o;
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
			if(strcmp(t.value, "->") == 0) {
				token_t next = _consume(par);
				opr = (operation_t){
					t.file, t.line, t.col, OPR_ASSIGN,
					next.len, next.value, parent, da_create(operation_t)
				};
			}
			else opr = (operation_t){
				t.file, t.line, t.col, OPR_BINARY,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		case TOK_UNARY:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_UNARY,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		case TOK_PRINT:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_PRINT,
				t.len, t.value, parent, da_create(operation_t)
			};
			break;
		case TOK_IF:
			opr = _if(par, parent, t);
			break;
		case TOK_KEYWORD:
			opr = _keyword(par, parent, t);
			if(opr.type == OPR_NULL) return 2;
			break;
		case TOK_LPAREN:
			opr = (operation_t){
				t.file, t.line, t.col, OPR_CODE,
				t.len, t.value, parent, da_create(operation_t)
			};

			while(!_parse(par, &opr));
			break;
		case TOK_RPAREN:
			return 1;
		default:
			break;
	}

	if(par->offset == par->tokens.nmem) {
		printf("[ERROR] : Unexpected EOF in parser.\n");
		return 2;
	}

	da_push(parent->children, opr, operation_t);
	return 0;
}

int parse_opr(parser_t *parser)
{
	return _parse(parser, parser->root);
}
