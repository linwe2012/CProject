#include <stdlib.h>
#include <stdio.h>
/*stacks: part of codes is from https://blog.csdn.net/huangchijun11/article/details/60963444 */
#define STACK_INI_SIZE 20
#define STACK_INCRETMENT 10

#define READ_MODE_FILE 0
#define READ_MODE_INSTANT 1

static int inputMode;
typedef char ElementType;
typedef struct
{
	ElementType *base;
	ElementType *top;
	int stackSize;
}operatorStack;

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

int stackLen(operatorStack *s)
{
	return (s->top - s->base);
}



