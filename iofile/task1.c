#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
  if (argc != 3) {
    printf("Invalid input\n");
    return -1;
  }

  FILE * fptr = fopen (argv[1], "r");
	if (fptr == NULL) {
		printf ("File does not exist\n");
		return -2;
	}
	int ch;
	int start = atoi (argv[2]);
	int i = 0;

	ch = getc (fptr);
	while (ch != EOF) {
		if (i >= start) {
			putchar (ch);
		}
		ch = getc (fptr);
		i++;
	}
  return 0;
}
