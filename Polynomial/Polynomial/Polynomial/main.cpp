#include <stdio.h>
#include "PolishReverse.h"
#include "IOControl.h"
#include "command.h"

int analysisString(char *s);

int main()
{
	OperatorStack opStack;
	FragmentStack fragStack;
	char buffer[MAX_BUFFER];
	int i;
	
	initCommand();

	while (1) {
		fgets(buffer, MAX_BUFFER, stdin);
		i = 0;
		while (buffer[i] == ' ')
			i++;
		if (*buffer == '\\') {
			cmdDealer(buffer + i);
		}
		else {
			if (analysisString(buffer) == 0) {
				initFragmentStack(&fragStack);
				initOperatorStack(&opStack);
				RPN(buffer, &opStack, &fragStack);
				printRPN(&fragStack, buffer);
			}
		}
	}

	return 0;
}

int analysisString(char *s) {
	int temp;
	
	compactArray(s, ' ');
	if (*(s + 1) == '=') {

	}
	else
	{
		if ((temp = parentheseCherker(s) )!= 0) {
			if (temp > 0) {
				printf("Expected right parenthese ')'.\n");
			}
			else 
				printf("Expected left parenthese '('.\n");
			return -1;
		}

		if (cmd_autoCorrect == CMD_TRUE) {
			expressionAutoCorrector(s);
		}
		else {
			if (stringLegalchecker(s) != 0) {
				printf("The expression contains illegal charater. Input again or turn on auto corrector\n");
				return -1;
			}
			else {
				;
			}
		}
	}
	return 0;
}

