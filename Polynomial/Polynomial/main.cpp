#include <stdio.h>
#include "PolishReverse.h"
#include "IOControl.h"


void compactArray(char *s, char c);

int main()
{
	char buffer[MAX_BUFFER];
	OperatorStack opStack;
	FragmentStack fragStack;

	fgets(buffer, MAX_BUFFER, stdin);
	compactArray(buffer, ' ');
	initFragmentStack(&fragStack);
	initOperatorStack(&opStack);
	RPN(buffer, &opStack, &fragStack);
	printRPN(&fragStack);

	getchar(); getchar(); getchar(); getchar();
	return 0;
}

void compactArray(char *s, char c)
{
	int i,j;
	for (i = 0, j = 0; s[i]; i++) {
		if (s[i] != c) {
			s[j++] = s[i];
		}
	}
	s[j] = s[i];
}