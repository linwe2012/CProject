#include <stdlib.h>
#include <stdio.h>
#include "PolishReverse.h"
/*part of the codes concerning stacks is from https://blog.csdn.net/huangchijun11/article/details/60963444 */
#define STACK_INI_SIZE 20
#define STACK_INCRETMENT 10

#define OPERATOR_PRIORITY_LEVEL 4
#define OPERATOR_MAX_SAME_PRIORIY 5


#define READ_MODE_FILE 0
#define READ_MODE_INSTANT 1

static int inputMode;
/*When an operator has a higher priority against the one at the top of the opStack 
* OR WHEN '(',')' are ALREADY at the top, it will be pushed into the opStack, 
* while others are popped from the opStack, and then pushed into fragStack, 
* which is why in this case parenthese are placed at lowest of them all.
*/
static const char operatorPrioriy[OPERATOR_PRIORITY_LEVEL][OPERATOR_MAX_SAME_PRIORIY] = { { '(', ')',    EMPTY_OPERATOR },
																							{'+', '-',   EMPTY_OPERATOR}, 
																							{'*',        EMPTY_OPERATOR}, 
																							{'^',        EMPTY_OPERATOR}, 
																							 };
int comparePriority(char a, char b);

void initOperatorStack(OperatorStack *s)
{
	s->base = (OperatorType *)malloc(STACK_INI_SIZE * sizeof(OperatorType));
	if (s == NULL) {
		printf("Fails to initialize operator stack.\n");
	}

	s->top = s->base;
	s->stackSize = STACK_INI_SIZE;
}

void pushOperatorStack(OperatorStack *s, OperatorType op)
{
	int stackSize = s->stackSize;
	if (s->top - s->base >= stackSize)
	{
		s->base = (OperatorType *)realloc(s->base, (stackSize + STACK_INCRETMENT) * sizeof(OperatorType));
		if (s == NULL) {
			printf("Fails to realloc operator stack.\n");
		}
		s->top = s->base + stackSize;
		s->stackSize = stackSize + STACK_INCRETMENT;
	}
	
	*(s->top) = op;
	s->top++;
}

void popOperatorStack(OperatorStack *s, OperatorType *op)
{
	if (s->top == s->base)
		return;
	*op = *--(s->top);
}

void deleteOperatorStack(OperatorStack *s)
{
	free(s);
}

OperatorType revealOperatorTop(OperatorStack *s)
{
	return *(s->top-1);
}

bool isEmptyOperatorStack(OperatorStack *s)
{
	if (s->top == s->base) {
		return true;
	}
	else
		return false;
}

int stackLen(OperatorStack *s)
{
	return (s->top - s->base);
}




void initFragmentStack(FragmentStack *s)
{
	s->base = (FragmentType *)malloc(STACK_INI_SIZE * sizeof(FragmentType));
	if (s == NULL) {
		printf("Fails to initialize Fragment stack.\n");
	}

	s->top = s->base;
	s->stackSize = STACK_INI_SIZE;
}

void pushFragmentStack(FragmentStack *s, FragmentType fragment)
{
	int stackSize = s->stackSize;
	if (s->top - s->base >= stackSize)
	{
		s->base = (FragmentType *)realloc(s->base, (stackSize + STACK_INCRETMENT) * sizeof(FragmentType));
		if (s == NULL) {
			printf("Fails to realloc Fragment stack.\n");
		}
		s->top = s->base + stackSize;
		s->stackSize = stackSize + STACK_INCRETMENT;
	}

	*(s->top) = fragment;
	s->top++;
}

void popFragmentStack(FragmentStack *s, FragmentType *fragment)
{
	if (s->top == s->base)
		return;
	*fragment = *--(s->top);
}

void deleteFragmenStack(FragmentStack *s)
{
	free(s);
}


int stackLen(FragmentStack *s)
{
	return (s->top - s->base);
}


void RPN(char *s, OperatorStack *op, FragmentStack *frag)
{
	OperatorType temp_op = NULL;
	bool hasNumberBeforeOperator = false;
	bool endStringFlag = false;

	while (!isStringEnd(*s) && !endStringFlag) {
		/*push pointer to numbers & variables into the stack*/
		if (isOperator(*s) == false) {
			pushFragmentStack(frag, s);
			while (isOperator(*s) == false) {
				s++;
				if (isStringEnd(*s)) {
					endStringFlag = true;
					break;
				}
			}
			hasNumberBeforeOperator = true;
		}
		if (endStringFlag) {
			break;
		}

		switch (*s) {
			case '(':{
				pushOperatorStack(op, s);
				hasNumberBeforeOperator = false;
				break;
			}
			case ')': {
				if (!isEmptyOperatorStack(op)) {
					popOperatorStack(op, &temp_op);
				}
				while (*temp_op != '(')
				{
					pushFragmentStack(frag, temp_op);
					if (!isEmptyOperatorStack(op)) {
						popOperatorStack(op, &temp_op);
					}
					else
					{
						break;
					}
				}
				break;
			}
			case '*': case'+': case'-': case'^': {
				if (isEmptyOperatorStack(op)) {
					pushOperatorStack(op, s);
				}
				else
				{
					temp_op = revealOperatorTop(op);
					/* *s has a higher priority over *temp_op*/
					if (*temp_op == '(' || *temp_op == ')' || comparePriority(*s, *temp_op) >= 1) {
						pushOperatorStack(op, s);
					}
					else
					{
						if (!isEmptyOperatorStack(op)) {
							do {
								popOperatorStack(op, &temp_op);
								pushFragmentStack(frag, temp_op);
								temp_op = revealOperatorTop(op);
							} while (!isEmptyOperatorStack(op) && comparePriority(*s, *temp_op) < 1);
						}
						pushOperatorStack(op, s);
					}
					/*
					if (hasNumberBeforeOperator = false) {

					}*/
				}
				break;
			}
			
		}
		s++;
	}
	while (!isEmptyOperatorStack(op)) {
		popOperatorStack(op, &temp_op);
		pushFragmentStack(frag, temp_op);
	}
}

bool isStringEnd(char c)
{
	if (c == '#' || c == '\n' || c == '\0') {
		return true;
	}
	else
		return false;
}
bool isOperator(char c) 
{
	if (c == '*' || c == '-' || c == '^' || c == '+' || c == '(' || c == ')' || c == '^') {
		return true;
	}
	else
		return false;
}
bool isNumber(char c)
{
	if (c <= '9' && c >= '0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int comparePriority(char a, char b) {
	int i, j;
	int a_priority, b_priority;
	for (i = 0; i < OPERATOR_PRIORITY_LEVEL; i++) {
		for (j = 0; j < OPERATOR_MAX_SAME_PRIORIY && operatorPrioriy[i][j] != EMPTY_OPERATOR; j++) {
			if (operatorPrioriy[i][j] == a) {
				a_priority = i;
			}
			if (operatorPrioriy[i][j] == b) {
				b_priority = i;
			}
		}
	}
	return a_priority - b_priority;
}

void printRPN(FragmentStack *frag) {
	char *s;
	FragmentType *moveable_Base;
	moveable_Base = frag->base;
	while ((moveable_Base < frag->top)) {
		s = *(moveable_Base);
		printf("%c", *s);
		while (!isOperator(*s)) {
			s++;
			if (isStringEnd(*s) || isOperator(*s)) {
				break;
			}
			printf("%c", *s);
		}
		printf(" ");
		(moveable_Base)++;
	}
}

