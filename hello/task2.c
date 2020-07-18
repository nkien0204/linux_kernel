#include <stdio.h>

int main() {
	FILE *fptr;
	fptr = fopen("README.txt", "r");
	int ch = getc(fptr);
	while (ch != EOF) {
		putchar(ch);
		ch = getc(fptr);
	}
	fclose(fptr);
	return 0;
}
