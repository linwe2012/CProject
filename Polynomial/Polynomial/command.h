#ifndef _COMMAND_H_
#define _COMMAND_H_
#define CMD_TRUE 1
#define CMD_FALSE 0
#define GREY 8
#define WHITE 7
#define LIGHT_WHITE
#define RED 4
#define BLUE 1
#define LIGHT_BLUE 9
#define YELLOW 6
#include "ExpressionSet.h"

extern int cmd_color;
extern int cmd_autoCorrect;
extern int cmd_autoParenthese;
extern int cmd_maxErrorLog;
extern int cmd_autoSave;

int initCommand();
int cmdDealer(char *s, ExpresionBuffer *expb);
void throwError(const char*errorLog, int color);

#endif // !_COMMAND_H_
