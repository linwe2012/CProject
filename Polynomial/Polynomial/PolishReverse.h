#ifndef _POLISHREVERSE_H_
#define _POLISHREVERSE_H_

#define EMPTY_OPERATOR 0
typedef char* OperatorType;
typedef struct
{
	OperatorType *base;
	OperatorType *top;
	int stackSize;
}OperatorStack;

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

void RPN(char *s, OperatorStack *op, FragmentStack *frag);
void initOperatorStack(OperatorStack *s);
void initFragmentStack(FragmentStack *s);
void printRPN(FragmentStack *frag);

#endif // !_POLISHREVERSE_H_
