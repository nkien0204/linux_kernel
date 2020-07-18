#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
	FILE * fptr = NULL;
	int ch;
	if (argc != 2) {
		printf ("ERROR, invalid input\n");
		return -1;
	}
	fptr = fopen (argv[1], "r");
	if (fptr == NULL) {
		return 0;
	}
	ch = getc(fptr);
	while (ch != EOF) {
		putchar(ch);
		ch = getc(fptr);
	}
	fclose (fptr);
	return 0;
}
