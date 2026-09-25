/*
 * CONCI - Conc Interpreter
 *
 *   Start Date   - 2026-09-25
 *   Last Updated - 2026-09-25
 *   Version      - 0.0.1
 *   Contributors - Tori A. Hall (toriahall@gmail.com)
 */

#include <stdio.h>
#include "../common/concb.h"
#include "../common/lexer.h"
#include "../common/parser.h"
#include "runtime.h"

int main(int argc, char** argv)
{
	if(argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	size_t size = 0;
	char *contents = readFile(argv[1], &size);

	if(!contents)
	{
		perror("conci");
		return 1;
	}

	lexer_t *lex = create_lexer(0, contents, size);
	while(!lex_token(lex));

	parser_t *par = create_parser(lex->tokens);
	while(!parse_opr(par));

	runtime_t *rt = create_runtime(par->root);
	run_program(rt);

	destroy_lexer(lex);
	destroy_parser(par);
	destroy_runtime(rt);
	free(contents);
	return 0;
}
