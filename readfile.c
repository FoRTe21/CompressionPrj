#include <stdio.h>

void main(int argc, char* argv[])
{
	FILE *fpInput = fopen(argv[1], "rb");

	int ch;
	while((ch = fgetc(fpInput)) != EOF)
	{
		printf("%d ", ch);
	}
		
	fclose(fpInput);
}
