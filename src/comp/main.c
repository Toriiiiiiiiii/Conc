/*
 * CONCC - Conc Compiler
 *
 *   Start Date   - 2026-10-10
 *   Last Updated - 2026-10-11
 *   Version      - 0.0.2
 *   Contributors - Tori A. Hall (toriahall@gmail.com)
 */

#include <stdio.h>
#include "../common/file.h"
#include "../common/lexer.h"
#include "../common/parser.h"
#include "asm_generator.h"

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
		perror("concc");
		return 1;
	}


	lexer_t *lex = create_lexer(0, contents, size);
	while(!lex_token(lex));

	parser_t *par = create_parser(lex->tokens);
	int res = 0;
	while(!(res = parse_opr(par)));

	if(res == 2) {
		printf("[CONCC] Parser error.\n");
		return 1;
	}

	asmgen_t *g = create_generator(par->root, "a.s");
	translate(g);

	destroy_lexer(lex);
	destroy_parser(par);
	destroy_generator(g);
	free(contents);

	char buf[256] = {0};
	sprintf(buf, "nasm -felf64 a.s -o %s", argv[2]);
	printf("%s\n", buf);
	res = system(buf);

	if(!res) {
		system("rm a.s");
	} else {
		printf("[CONCC] An error occured.\n");
		return 1;
	}

	return 0;
}
