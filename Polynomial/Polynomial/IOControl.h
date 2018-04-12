#ifndef _IOCONTROL_H_
#define _IOCONTROL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 100
#define MAX_ADDRESS 100
#define MAX_BUFFER 1000

char* freadExpression(char *buffer, char *address, int maxBuffer);

#endif // !_IOCONTROL_H_

