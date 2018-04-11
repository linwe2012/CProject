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

void initOperatorStack(operatorStack *s)
{
	s->base = (ElementType *)malloc(STACK_INI_SIZE * sizeof(operatorStack));
	if (s == NULL) {
		printf("Fails to initialize operator stack.\n");
	}

	s->top = s->base;
	s->stackSize = STACK_INI_SIZE;
}

void pushOperatorStack(operatorStack *s, ElementType op)
{
	int stackSize = s->stackSize;
	if (s->top - s->base >= stackSize)
	{
		s->base = (ElementType *)realloc(s->base, (stackSize + STACK_INCRETMENT) * sizeof(operatorStack));
		if (s == NULL) {
			printf("Fails to realloc operator stack.\n");
		}
		s->top = s->base + stackSize;
		s->stackSize = stackSize + STACK_INCRETMENT;
	}
	
	*(s->top) = op;
	s->top++;
}

void popOperatorStack(operatorStack *s, ElementType *op)
{
	if (s->top == s->base)
		return;
	*op = *--(s->top);
}

void deleteOperatorStack(operatorStack *s)
{
	free(s);
}

ElementType revealOperatorTop(operatorStack *s)
{
	return *(s->top);
}

bool isEmptyOperatorStack(operatorStack *s) 
{
	if (s->top == s->base) {
		return true;
	}
	else
		return false;
}

int stackLen(operatorStack *s)
{
	return (s->top - s->base);
}




void initFragmentStack(FragmentStack *s)
{
	s->base = (FragmentType *)malloc(STACK_INI_SIZE * sizeof(FragmentStack));
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
		s->base = (FragmentType *)realloc(s->base, (stackSize + STACK_INCRETMENT) * sizeof(FragmentStack));
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


void RPN(char *s, operatorStack *op, FragmentStack *frag)
{
	ElementType temp_op = NULL;
	bool hasNumberBeforeOperator = false;

	while (!isStringEnd(*s)) {
		while (*s == ' ') {
			s++;
		}
		/*push pointer to numbers & variables into the stack*/
		if (isNumber(*s)) {
			pushFragmentStack(frag, s);
			while (isOperator(*s) == false) {
				s++;
				while (*s == ' ') {
					s++;
				}
			}
			hasNumberBeforeOperator = true;
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
				}
				break;
			}
			case '*': case'+': case'-': {
				if (isEmptyOperatorStack(op)) {
					pushOperatorStack(op, s);
				}
				temp_op = revealOperatorTop(op);
				/* *s has a higher priority over *temp_op*/
				if (*temp_op == '(' || *temp_op == ')' || comparePriority(*s, *temp_op) >= 1) {
					pushOperatorStack(op, s);
				}
				else
				{
					
					do {
						popOperatorStack(op, &temp_op);
						pushFragmentStack(frag, temp_op);
						temp_op = revealOperatorTop(op);
					} while (comparePriority(*s, *temp_op) < 1 && isEmptyOperatorStack(op));
				}
				/*
				if (hasNumberBeforeOperator = false) {

				}*/
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
	if (c == '*' || c == '-' || c == '^' || c == '+') {
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
			printf("%c", *s);
			s++;
		}
		(moveable_Base)++;
	}
}