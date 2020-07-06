#include <stdio.h>

extern char ** environ;

int main () {
	int count = 0;
	while (environ[count] != NULL) {
		printf ("%s\n", environ[count]);
		count++;
	}
	return 0;
}
