#ifndef _POLISHREVERSE_H_
#define _POLISHREVERSE_H_

#define EMPTY_OPERATOR 0
typedef char* ElementType;
typedef struct
{
	ElementType *base;
	ElementType *top;
	int stackSize;
}operatorStack;

typedef char* FragmentType;
typedef struct
{
	FragmentType *base;
	FragmentType *top;
	int stackSize;
}FragmentStack;


bool isNumber(char c);
bool isOperator(char c);
bool isStringEnd(char c);

void RPN(char *s, operatorStack *op, FragmentStack *frag);
void initOperatorStack(operatorStack *s);
void initFragmentStack(FragmentStack *s);
void printRPN(FragmentStack *frag);

#endif // !_POLISHREVERSE_H_
