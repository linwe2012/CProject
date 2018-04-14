#include <stdio.h>
#include <windows.h>
#include "PolishReverse.h"
#include "IOControl.h"
#include "command.h"
#include "ExpressionSet.h"

int analysisString(char *s);
void initialCongratulate();

int main()
{
	OperatorStack opStack;
	FragmentStack fragStack;
	ExpresionBuffer expBuffer;
	char buffer[MAX_BUFFER];
	int i = 0;
	
	initCommand();
	initialCongratulate();
	initExpressionBuffer(&expBuffer);


	while (1) {
		fgets(buffer, MAX_BUFFER, stdin);
		if (*buffer == '\\') {
			i = cmdDealer(buffer + 1, &expBuffer);
			if (i >= 0) {
				//printExpressionID(&expBuffer);
			}
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
		
		if (cmd_autoCorrect == CMD_TRUE) {
			expressionAutoCorrector(s);
		}
		temp = parentheseCherker(s);
		if (cmd_autoParenthese == CMD_TRUE) {
			if ((temp = parentheseAutoAdder(s, temp, MAX_BUFFER)) == -1) {
				return -1;
			}
			else if(temp == 1)
			{
				throwError("analysisString::Elements are auto added. the Expresion is currently:\n", GREY);
				puts(s);
			}
		}
		else if (temp != 0) {
			if (temp > 0)
				throwError("analysisString::Expected right parenthese ')'.\n", GREY);
			else
				throwError("analysisString::Expected left parenthese '('.\n", GREY);
			return -1;
		}

		if(cmd_autoCorrect == CMD_FALSE || cmd_autoParenthese == CMD_FALSE) {
			if (stringLegalchecker(s) != 0) {
				throwError("analysisString::The expression contains illegal charater. Input again or turn on auto corrector\n", GREY);
				return -1;
			}
			else {
				;
			}
		}
	}
	return 0;
}

void initialCongratulate() 
{
	printf("You can input any exprssion.\n");
	printf("Operators this program supports: ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_BLUE);
	printf("-");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_BLUE);
	printf("+");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_BLUE);
	printf("*");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_BLUE);
	printf("^");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf(".\n");
	printf("If you have input anything wrong, the program will try to correct it\n");
	printf("Type in \'\\help\' to get setting list.\n");
}