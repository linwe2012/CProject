#include "ExpressionSet.h"
#include "command.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
int expressionSetsSize = sizeof(ExpressionSets);
int expressionBufferSize = sizeof(ExpresionBuffer);

void initExpressionBuffer(ExpresionBuffer *expb)
{
	expb->base = (ExpressionSets *)malloc(EXPRESSION_MAX_BUFFER * expressionSetsSize);
	if (expb == NULL) {
		throwError("initExpressionBuffer::fails to initialize expresion buffer. The program will end.\n", RED);
		getchar(); getchar(); getchar(); getchar();
		exit(0);
	}
	expb->top = expb->base;
	expb->maxSize = EXPRESSION_MAX_BUFFER;
	expb->currentID = 0;
	expb->offset = 0;
}



