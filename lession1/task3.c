#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
	int pid;
	int ch;
	char buff[255];
	FILE * fptr = NULL;
	if (argc != 2) {
		printf ("ERROR, invalid PID input\n");
		return -1;
	}
	pid = atoi (argv[1]);
	sprintf(buff, "/proc/%d/cmdline", pid);
	fptr = fopen(buff, "r");
	if (fptr == NULL) {
		printf ("Process does not exist\n");
		return -2;
	}
	ch = getc(fptr);
	while(ch != EOF) {
		putchar(ch);
		ch = getc(fptr);
	}
	printf("\n");
	fclose(fptr);
	return 0;
}
