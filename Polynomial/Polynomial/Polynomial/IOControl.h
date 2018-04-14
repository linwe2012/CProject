#ifndef _IOCONTROL_H_
#define _IOCONTROL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExpressionSet.h"


#define MAX_ADDRESS 100
#define MAX_BUFFER 1000
#define DEFAULT_SAV_FILE_LOCATION "./res/sav.exp"
#define DEFAULT_SAV_FILE_NAME "sav.exp"
#define DEFALT_TEMP_FILE_LOCATION "./res/temp.exp"
#define DEFALT_BUFFER_FILE_LOCATION "./res/temp.exp"

/**
* @brief: 
* @param [in] exp1: 
* @param [in] exp2:
* @exception
* @note
*/
char* fgetExpression(char *buffer, char *address);
/**
* @brief: check if it is a legal expression
* @param [in] s: a pointer to the string to be check;
* @exception
* @return 0:is Totally Legal, which contains only variables, number and operater
		  1:is partly legal, which contains blanks and other legal stuff
* @note
*/
int stringLegalchecker(char*s);
void compactArray(char *s, char c);
void expressionAutoCorrector(char *s);
int parentheseCherker(char *s);
int parentheseAutoAdder(char *s, int num, int maxBuffer);


bool isFundamentalOperator(char c);
ExpressionSets *freadExpression(int ID, const char *FileAddress);
void throwError(const char*errorLog, int color);


void printExpressionSet(ExpressionSets *exps);
int printExpressionBuffer(ExpresionBuffer *expb);
bool checkExpressionBufferID(ExpresionBuffer *expb, int ID);
void printExpressionID(ExpresionBuffer *expb);

#endif // !_IOCONTROL_H_

