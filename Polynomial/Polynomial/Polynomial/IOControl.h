#ifndef _IOCONTROL_H_
#define _IOCONTROL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 100
#define MAX_ADDRESS 100
#define MAX_BUFFER 1000
/**
* @brief: 
* @param [in] exp1: 
* @param [in] exp2:
* @exception
* @note
*/
char* freadExpression(char *buffer, char *address);
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
#endif // !_IOCONTROL_H_

