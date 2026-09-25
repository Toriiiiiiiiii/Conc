#include <stdio.h>
#include "../common/file.h"

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

	printf("%s\n", contents);
	return 0;
}
