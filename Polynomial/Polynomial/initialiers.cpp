#include "initializer.h"
#include <Windows.h>
#include <io.h>
#include <direct.h>

void initIOControl()
{
	char dirName[10] = "./src";
	if (_access(dirName, 0) == -1) {
		_mkdir(dirName);
		if (_access(dirName, 0) == -1) {
			std::cerr << "initIOControl::Create directory failed, may fails to record";
		}
	}
}






/*set font code from here: https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/c87c4bc1-b25c-4031-a062-32edbd16a75f/console-application-font-size-type-etc?forum=windowsgeneraldevelopmentissues */
/*
void setMyCurrentConsolefont()
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX lpConsoleCurrentFontEx;
	lpConsoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	lpConsoleCurrentFontEx.dwFontSize.X = 8;
	lpConsoleCurrentFontEx.dwFontSize.Y = 15;
	lpConsoleCurrentFontEx.FontWeight = 100;
	lpConsoleCurrentFontEx.nFont = 1;
	lpConsoleCurrentFontEx.FontFamily = FF_DECORATIVE;
	lstrcpyW(lpConsoleCurrentFontEx.FaceName, L"Ariel");

	int Status;
	Status = SetCurrentConsoleFontEx(hOut, false, &lpConsoleCurrentFontEx);

	if (Status == 0) //skipped
	{
		Status = GetLastError();
		std::cout << "SetConsoleTitle() failed! Reason : " << Status << std::endl;
		exit(Status);
	}
}*/