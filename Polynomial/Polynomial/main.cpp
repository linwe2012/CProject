#include <stdio.h>
#include <windows.h>
#include <math.h>
//#include <pthread.h>
#include "PolishReverse.h"
#include "IOControl.h"
#include "command.h"
#include "ExpressionSet.h"
#include "PolyCalculator.h"
#include "initializer.h"
#include "errorDealer.h"

//#pragma comment(lib,"pthreadVC2.lib") 

//struct parameter {
//	FragmentStack *frag;
//	ExpresionBuffer *expb;
//} para;
//(x+y)(x-y)(a+b+c)
int analysisString(char *s);
void initialCongratulate();
int cal(FragmentStack *frag, ExpresionBuffer *expb);
//void thread1(struct parameter *para);

int main()
{
	OperatorStack opStack;
	FragmentStack fragStack;
	ExpresionBuffer expBuffer;

	ExpressionSets *exps1;
	Expressions *exp1;
	
	char buffer[MAX_BUFFER];
	char tempBuffer[MAX_BUFFER];
	int i = 0;

	//pthread_t thd1;
	
	initCommand();
	initialCongratulate();
	initExpressionBuffer(&expBuffer);

	initFragmentStack(&fragStack);
	initOperatorStack(&opStack);

	initIOControl();

	cleanUp(true, &opStack, &fragStack);
	//setMyCurrentConsolefont();

	setSignalHandler();
	setjmp(jmp_to_begin);

	while (1) {
		fgets(tempBuffer, MAX_BUFFER, stdin);
		if (*tempBuffer == '\\') {
			i = cmdDealer(tempBuffer + 1, &expBuffer, &fragStack, buffer);
			if (i >= 0) {
				//printExpressionID(&expBuffer);
			}
		}
		else {
			compactArray(tempBuffer, ' ');
			if (*(tempBuffer + 1) == '=') {
				//printf("%s", tempBuffer);
				varValueControl(getCurrentBufferExpressionSets(&expBuffer), &expBuffer, tempBuffer);
			}
			else {
				strcpy_s(buffer, tempBuffer);
				if (analysisString(buffer) == 0) {
					clearOperatorStack(&opStack);
					clearFragmentStack(&fragStack);
					clearVarValue();
					RPN(buffer, &opStack, &fragStack);
					//printRPN(&fragStack, buffer);

					cal(&fragStack, &expBuffer);
					exps1 = getCurrentBufferExpressionSets(&expBuffer);
					exp1 = expressionDuplicate(*(exps1->base));
					//printExpression(exp1);
					//printBufferCurrentOffset(&expBuffer);
				}
			}
			
		}
	}

	return 0;
}

int analysisString(char *s) {
	int temp;
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
			throwError("analysisString::The expression contains illegal character. Input again or turn on auto-corrector\n", GREY);
			return -1;
		}
		else {
			;
		}
	}
	return 0;
}

void initialCongratulate() 
{
	HANDLE hwdl = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("You can input any exprssion.\n");
	printf("Operators this program supports: ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("-");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("+");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("*");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("^");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(".\n");
	printf("If you have input anything wrong, the program will try to correct it\n");
	printf("Type in \'\\help\' to get setting list.\n");
}


//1+2x+3x^2+4x
int cal(FragmentStack *frag,ExpresionBuffer *expb)
{
	FragmentType *moveable_Base;
	static Expressions *exp1, *exp2, *exp_temp, *exp3;
	ExpressionSets *exps;
	static ExpressionSets *expStack;
	char c;
	int num;
	initVar();
	cleanUpHalfTime(true, &exp1, &exp2, &exp_temp, &exp3, &expStack);
	exps = initExpressiosSets(true,NULL);
	expStack = initExpressiosSets(false, NULL, 5);
	moveable_Base = frag->base;

	exp_temp = newExpression(*moveable_Base);
	pushExpressiosSets(expStack, exp_temp);
	moveable_Base++;
	while ((moveable_Base < frag->top)) {
		if (isOperator(**moveable_Base)) {
			c = **moveable_Base;
			if (c == '^') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				num = exp2->coeff;
				freeExpression(exp2);
				if (num == 1) {
					pushExpressiosSets(expStack, exp1);
				}
				else if (exp1->next == NULL && exp1->son->son == NULL) {
					exp1->son->degree *= num;
					exp1->coeff = int(pow(exp1->coeff, num));
					pushExpressiosSets(expStack, exp1);
				}
				else {
					//exp2 = expressionDuplicate(exp1);
					exp3 = exp2 = mul(exp1, exp1);
					//printf("(^)expStack = ");
					//printExpressionSet_all(expStack);
					//printf("^\n");

					for (; num > 2; num--) {
						exp3 = mul(exp2, exp1);
						freeExpression(exp2);
						exp2 = exp3;
					}
					pushExpressiosSets(expStack, exp3);
				}
				
				
			}
			else if (c == '+') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				//printf("prepare add ");
				add(exp1, exp2);
				//printf("add finish");
				//printf("(+)exp1 = ");
				//1+(x+2y)^2
				//printExpression(exp1);
				pushExpressiosSets(expStack, exp1);

				//printf("(+)expStack = ");
				//printExpressionSet_all(expStack);

				exp1 = NULL;
				//printf("+\n");
			}
			//1+(5x+6)-7abc(dsjhhu+sgh)
			else if (c == '-') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				sub(exp1, exp2);
				pushExpressiosSets(expStack, exp1);

				//printf("(-)expStack = ");
				//printExpressionSet_all(expStack);
				//printf("-\n");
			}
			else if (c == '*') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				exp3 = mul(exp1, exp2);
				pushExpressiosSets(expStack, exp3);

				//printf("(*)expStack = ");
				//printExpressionSet_all(expStack);

				freeExpression(exp1);
				freeExpression(exp2);
				exp3 = NULL;
				//printf("*\n");
			}
			else if (c == '~') {
				exp1 = (*(expStack->top - 1));
				while (exp1) {
					exp1->coeff =- exp1->coeff;
					exp1 = exp1->next;
				}
			}
		}
		else {
			exp_temp = newExpression(*moveable_Base);
			pushExpressiosSets(expStack, exp_temp);
			//printf("(add)expStack = ");
			//printExpressionSet_all(expStack);
			//printf("size = %d  \n", expStack->top - expStack->base);
		}
		moveable_Base++;
	}
	pushExpressiosSets(exps, *(expStack->base));
	expressionSetsVarCpy(exps);
	//lift its base so that the expression at the base won't be freed
	(expStack->base)++;
	freeExpressionSets(expStack);
	pushExpressionBuffer(expb, exps, true);
	printBufferCurrentOffset(expb);
	//cmdDraw(0, 1, 1, 50, 0, 0, exps);
	return 0;
}

//void thread1(struct parameter *para)
//{
//	cal(para->frag, para->expb);
//}