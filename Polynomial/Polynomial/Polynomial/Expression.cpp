#include "Expression.h"
#include "PolishReverse.h"
#include <stdlib.h>
#include <stdio.h>

int newVar(char v, bool *isNewVar);
PolyType getNextNum(char *s, int *leap);

Expressions *newExpression(char *s, char *head){
	Expressions *newexp = NULL;
	//Poly *polyptr, *polyptr_last;
	int leap;
	//case like abc^8abc or abc^a88
	if (*(s - 1) == '^' && s != head && isNumber(*s) != isNumber(*(s + 1)) && !isOperator(*(s + 1))) {
		if (isNumber(*s)) {
			newexp = (Expressions *)malloc(ExpressionSize);
			newexp->coeff = getNextNum(s, &leap);
			newexp->next = NULL;
			newexp->son = NULL;
		}
		else {
			printf("Illegal Expression: %c", *s);
		}
	}
	else
	{
		newexp = (Expressions *)malloc(ExpressionSize);
		newexp->coeff = 1;
		newexp->next = NULL;
		newexp->son = NULL;
		while (!isOperator(*s)) {
			if (isNumber(*s)) {
				newexp->coeff *= getNextNum(s, &leap);
				if (newexp->coeff == 0) {
					freePolyList(newexp->son);
					return newexp;
				}
			}
			else {

			}
			s++;
		}
	}

	return newexp;
}

int newVar(char v, bool *isNewVar)
{
	int i = 0;
	while (varTable[i] != -1) {
		if (varTable[i] == v) {
			*isNewVar = false;
			return i;
		}
		i++;
	}
	varTable[i] = v;
	varTable[i + 1] = -1;
	*isNewVar = true;
	return i;
}

PolyType getNextNum(char *s, int *leap) {
	PolyType num = 0;
	PolyType denominator = 1;
	PolyType isBeforeDot = 10;
	*leap = 0;

	while (isNumber(*s) == true) {
		num = num * isBeforeDot + (*s - '0') / denominator;
		if (*s == '.' || denominator > 1) {
			denominator = denominator * 10;
			isBeforeDot = 1;
		}
	}
	return num;
}

void freePolyList(Poly *head) {
	Poly *ptr;
	while (head) {
		ptr = head;
		head = head->son;
		free(ptr);
	}
}

void freeExpression(Expressions *head) {
	Expressions *exp;
	while (head) {
		freePolyList(head->son);
		exp = head;
		head = head->next;
		free(exp);
	}
}

int addNewVariable(char var, Expressions *exp) {
	bool isNewVar;
	int varPriority = newVar(var, &isNewVar);
	Poly *newVarNode, *ptr;
	Poly *head = exp->son;

	newVarNode = (Poly *)malloc(PolySize);
	newVarNode->degree = 1;
	newVarNode->var = varPriority;

	if (isNewVar == true) {
		while (head->son != NULL) {
			head = head->son;
		}
		newVarNode->son = NULL;
	}
	else {
		
		newVarNode = head;
		if (head->var > varPriority) {
			newVarNode->son = head;
			exp->son = newVarNode;
		}
		else {
			ptr = head;
			while (head) {
				if (head->var == var) {
					head->degree += 1;
					break;
				}
				else if (ptr->var < varPriority && head->var > varPriority) {
					newVarNode->son = head;
					ptr->son = newVarNode;
					break;
				}
				else {
					head = head->son;
				}
			}
		}
		
	}
	return 0;
}