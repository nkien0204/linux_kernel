#include "calculate.h"

int Add (int a, int b) {
	return a + b;
}

int Subtract (int a, int b) {
	return a - b;
}

int Mul (int a, int b) {
	return a * b;
}

float Div (int a, int b) {
	if (b == 0) {
		printf ("Could not divide for 0\n");
		exit(0);
	}
	return (float)a / b;
}
