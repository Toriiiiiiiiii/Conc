/*
 * CONCC - Conc Compiler
 *
 *   Start Date   - 2026-10-10
 *   Last Updated - 2026-10-10
 *   Version      - 0.0.1
 *   Contributors - Tori A. Hall (toriahall@gmail.com)
 */

#include <stdio.h>
#include "../common/file.h"
#include "../common/lexer.h"
#include "../common/parser.h"

int main(int argc, char **argv)
{
	if(argc < 3) {
		printf("Usage: %s <filename> <output>\n", argv[0]);
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
	int res = 0;
	while(!(res = parse_opr(par)));

	if(res == 2) return 1;

	destroy_lexer(lex);
	destroy_parser(par);
	free(contents);
	return 0;
}
