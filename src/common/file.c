#include "file.h"

char *readFile(char *path, size_t *size)
{
	FILE *file = fopen(path, "r");
	if(!file) return NULL;

	fseek(file, 0, SEEK_END);
	*size = ftell(file) + 1;
	fseek(file, 0, SEEK_SET);

	char *buf = malloc(*size);
	if(!buf) return NULL;

	fread(buf, 1, *size-1, file);
	fclose(file);
	return buf;
}
