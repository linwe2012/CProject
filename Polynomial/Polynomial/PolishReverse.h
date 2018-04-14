#ifndef _POLISHREVERSE_H_
#define _POLISHREVERSE_H_

#define EMPTY_OPERATOR 0
typedef const char* OperatorType;
typedef struct
{
	OperatorType *base;
	OperatorType *top;
	int stackSize;
}OperatorStack;

typedef const char* FragmentType;
typedef struct
{
	FragmentType *base;
	FragmentType *top;
	int stackSize;
}FragmentStack;

const char autoMultiplyOperator[2] = "*";
//if a minus operater is a unary
const char negationIdentifier[2] = "~";

bool isNumber(char c);
bool isOperator(char c);
bool isVariable(char c);
bool isStringEnd(char c);

void RPN(char *s, OperatorStack *op, FragmentStack *frag);
void initOperatorStack(OperatorStack *s);
void initFragmentStack(FragmentStack *s);
void printRPN(FragmentStack *frag, char *head);

#endif // !_POLISHREVERSE_H_
