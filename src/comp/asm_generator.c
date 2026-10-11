#include "asm_generator.h"
#include "../common/file.h"

asmgen_t *create_generator(operation_t *program, char *path)
{
	asmgen_t *g = malloc(sizeof(asmgen_t));

	g->root = program;
	g->outp_path = path;

	g->fns = da_create(operation_t);

	g->ifs = 0;
	g->cmps = 0;

	g->file = fopen(path, "w");
	return g;
}

void destroy_generator(asmgen_t *g)
{
	fclose(g->file);
	free(g);
}


void _trans_node(asmgen_t *g, operation_t opr)
{
	if(opr.type == OPR_CODE) {
		for(int i = 0; i < opr.children.nmem; ++i)
		{
			_trans_node(g, da_peek(opr.children, i, operation_t));
		}
	} else if(opr.type == OPR_PUSH_INT)
	{
		char *buf = malloc(16777216);
		sprintf(buf, "\tmov rax, %s\n\tcall stk_push\n", opr.value);
		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);
	} else if(opr.type == OPR_BINARY)
	{
		char *buf = malloc(16777216);

		switch(opr.value[0]) {
			case '+':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tadd rax, rdi\n\tcall stk_push\n");
				break;
			case '-':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tsub rax, rdi\n\tcall stk_push\n");
				break;
			case '*':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tmul rdi\n\tcall stk_push\n");
				break;
			case '/':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tdiv rdi\n\tcall stk_push\n");
				break;
			case '%':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tdiv rdi\n\tmov rax, rdx\n\tcall stk_push\n");
				break;
			case '<':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tcmp rax, rdi\n\tjge FL_%lu\n\tmov rax, 1\n\tcall stk_push\n\tjmp CMP_%lu_DN\nFL_%lu:\n\tmov rax, 0\n\tcall stk_push\nCMP_%lu_DN:\n", g->cmps, g->cmps, g->cmps, g->cmps);
				g->cmps++;
				break;
			case '>':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tcmp rax, rdi\n\tjle FL_%lu\n\tmov rax, 1\n\tcall stk_push\n\tjmp CMP_%lu_DN\nFL_%lu:\n\tmov rax, 0\n\tcall stk_push\nCMP_%lu_DN:\n", g->cmps, g->cmps, g->cmps, g->cmps);
				g->cmps++;
				break;
			case '=':
				sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall stk_pop\n\tcmp rax, rdi\n\tjne FL_%lu\n\tmov rax, 1\n\tcall stk_push\n\tjmp CMP_%lu_DN\nFL_%lu:\n\tmov rax, 0\n\tcall stk_push\nCMP_%lu_DN:\n", g->cmps, g->cmps, g->cmps, g->cmps);
				g->cmps++;
				break;
		}

		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);
	} else if(opr.type == OPR_UNARY) {
		char *buf = malloc(16777216);

		switch(opr.value[0]) {
			case ':':
				sprintf(buf, "\tcall stk_pop\n\tcall stk_push\n\tcall stk_push\n");
				break;
			case '@':
				sprintf(buf, "\tcall stk_pop\n");
				break;
		}

		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);
	} else if(opr.type == OPR_PRINT)
	{
		char *buf = malloc(16777216);
		sprintf(buf, "\tcall stk_pop\n\tmov rdi, rax\n\tcall print_int\n");
		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);
	} else if(opr.type == OPR_PROC)
	{
		da_push(g->fns, opr, operation_t);
	} else if(opr.type == OPR_KEYWORD)
	{
		operation_t fn = {0};

		for(int i = 0; i < g->fns.nmem; ++i)
		{
			fn = da_peek(g->fns, i, operation_t);
			if(strcmp(opr.value, fn.value) == 0) break;

			fn = (operation_t){0};
		}

		if(fn.type != OPR_PROC) {
			printf("[ERROR] Unknown reference to '%s'\n", opr.value);
			return;
		}

		char *buf = malloc(16777216);
		sprintf(buf, "\tcall FN_%s\n", opr.value);
		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);
	} else if(opr.type == OPR_IF)
	{
		_trans_node(g, da_peek(opr.children, 0, operation_t));
		
		char *cond = "\tcall stk_pop\n\tcmp rax, 0\n";
		fwrite(cond, 1, strlen(cond), g->file);

		if(opr.children.nmem >= 3) {
			char *buf = malloc(16777216);
			sprintf(buf, "\tje ELSE_%lu\n", g->ifs);
			fwrite(buf, 1, strlen(buf), g->file);
			free(buf);
		} else {
			char *buf = malloc(16777216);
			sprintf(buf, "\tje END_%lu\n", g->ifs);
			fwrite(buf, 1, strlen(buf), g->file);
			free(buf);
		}

		_trans_node(g, da_peek(opr.children, 1, operation_t));
		char *end = malloc(16777216);
		sprintf(end, "\tjmp END_%lu\n", g->ifs);
		fwrite(end, 1, strlen(end), g->file);
		free(end);

		if(opr.children.nmem >= 3) {
			char *buf = malloc(16777216);
			sprintf(buf, "ELSE_%lu:\n", g->ifs);
			fwrite(buf, 1, strlen(buf), g->file);
			free(buf);

			_trans_node(g, da_peek(opr.children, 2, operation_t));
		}

		char *buf = malloc(16777216);
		sprintf(buf, "END_%lu:\n", g->ifs);
		fwrite(buf, 1, strlen(buf), g->file);
		free(buf);

		g->ifs++;
	}
}

void translate(asmgen_t *g)
{
	size_t size = 0;
	char *header = readFile("templates/header.asm", &size);
	fwrite(header, 1, strlen(header), g->file);

	_trans_node(g, *(g->root));

	char *foot = "\tmov rdi, 0\n\tmov rax, 60\n\tsyscall\n\tmov rsp, rbp\n\tpop rbp\n\tret\n";
	fwrite(foot, 1, strlen(foot), g->file);

	for(int i = 0; i < g->fns.nmem; ++i)
	{
		operation_t opr = da_peek(g->fns, i, operation_t);
		char *label = malloc(16777216);

		sprintf(label, "FN_%s:\n\tpush rbp\n\tmov rbp, rsp\n", opr.value);
		fwrite(label, 1, strlen(label), g->file);
		free(label);

		_trans_node(g, da_peek(opr.children, 0, operation_t));

		label = "\tmov rsp, rbp\n\tpop rbp\n\tret\n";
		fwrite(label, 1, strlen(label), g->file);
	}

	char *footer = readFile("templates/footer.asm", &size);
	fwrite(footer, 1, strlen(footer), g->file);
}
