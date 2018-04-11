#include <stdio.h>
#include "PolishReverse.h"

#define MAX_BUFFER 1000

int main()
{
	char buffer[MAX_BUFFER];
	operatorStack opStack;
	FragmentStack fragStack;

	scanf_s("%s", buffer);
	initFragmentStack(&fragStack);
	initOperatorStack(&opStack);
	RPN(buffer, &opStack, &fragStack);
	printRPN(&fragStack);

	getchar(); getchar(); getchar(); getchar();
	return 0;
}