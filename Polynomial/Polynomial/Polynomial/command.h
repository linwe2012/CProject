#ifndef _COMMAND_H_
#define _COMMAND_H_
#define CMD_TRUE 1
#define CMD_FALSE 0
#define GREY 8
#define WHITE 7

extern int cmd_color;
extern int cmd_autoCorrect;

int initCommand();
int cmdDealer(char *s);


#endif // !_COMMAND_H_
