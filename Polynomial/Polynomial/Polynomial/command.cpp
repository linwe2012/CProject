#include "command.h"


int cmd_color = CMD_TRUE;
int cmd_autoCorrect= CMD_TRUE;


char cmd_list[][20] = {
	"\\ac on\\off",  //auto correct
	"\\color on\\off",
	"\\u (num)",    //trace back in history
	"\\d (num)",
	"\\loc num",     //locate at a expression
	"\\sav name",    //save the result
	"\\load name",
	"\\read address name",
	"\\shw set",     //show current settings
	"\\shw buf",    //show current buffer
	"\\shw prn",    //show prn
};

char cmd_explain[][100] = {
	""
};

int cmdDealer(char *s) {
	return 0;
}

int initCommand() {
	cmd_color = CMD_TRUE;
	cmd_autoCorrect = CMD_TRUE;
	return 0;
}