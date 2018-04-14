#ifndef _EXPRESSIONSET_H_
#define _EXPRESSIONSET_H_
#include "Expression.h"

#define EXPRESSION_MAX_BUFFER 5

#define MAX_NAME 20

typedef struct {
	Expressions *base;
	Expressions *top;
	int maxSize;
	int ID;
	char name[MAX_NAME];
}ExpressionSets;

typedef struct {
	ExpressionSets *base;
	ExpressionSets *top;
	int maxSize;
	int currentID;
	int offset;
}ExpresionBuffer;

extern int expressionSetsSize;
extern int expressionBufferSize;

void addExpressionBuffer(ExpresionBuffer *expb);

void initExpressionBuffer(ExpresionBuffer *expb);

#endif // !_EXPRESSIONSET_H_

