#include "IOControl.h"
#include "PolishReverse.h"
#include <windows.h>
#include "command.h"
static FILE *globe_fp;

char* freadExpression(char *buffer, char *address) 
{
	static char exAddress[MAX_ADDRESS];
	static long offset;
	static char name[MAX_NAME];
	char temp_c;
	FILE *fp;
	//have the same value
	if (strcmp(exAddress, address) != 0) {
		offset = 0;
	}
	fopen_s(&fp, address, "r");
	if (fp == NULL) {
		printf("Fails to read File::Address: %s, exAdress, %s,  offset: %ld\n", address, exAddress,  offset);
	}
	fseek(fp, offset, SEEK_SET);
	while ((temp_c = fgetc(fp)) == '\n' || temp_c == ' ') 
		;
	if (feof(fp)) {
		return NULL;
	}
	else {
		if (temp_c != '#') {
			printf("Wrong Format::Address: %s, exAdress, %s, offset: %ld\n"
				   "read in charater: %c\n", address, exAddress,  offset, temp_c);
		}
		else
		{
			if ((temp_c = fgetc(fp)) == '\n') {
				name[0] = '\0';
			}
			else
			{
				name[0] = temp_c;
				fgets(name + 1, MAX_NAME - 1, fp);
			}
		}
		while ((temp_c = fgetc(fp)) == '\n' || temp_c == ' ')
			;
		if (temp_c != '$') {
			printf("Wrong Format::Address: %s, exAdress, %s,  offset: %ld\n"
				"read in charater: %c\n", address, exAddress, offset, temp_c);
		}
		else {
			buffer[0] = temp_c;
			fgets(buffer + 1, MAX_BUFFER - 1, fp);
			return name;
		}
	}
	return NULL;
}


int stringLegalchecker(char*s) 
{
	int legal = 0;
	char c;
	c = *s;
	while (!isStringEnd(c)) {
		if (isNumber(c) || isOperator(c) || isVariable(c)) {
			;
		}
		else if (c == ' ') {
			legal = 1;
		}
		else {
			legal = -1;
			return legal;
		}
		c = *++s;
	}
	return legal;
}

int parentheseCherker(char *s)
{
	int left = 0, right = 0;
	char c = *s;
	while (!isStringEnd(c)) {
		if (c == '('){
			left++;
		}
		else if(c == ')')
		{
			right++;
		}
		c = *++s;
	}
	return left - right;
}

void compactArray(char *s, char c)
{
	int i, j;
	for (i = 0, j = 0; s[i]; i++) {
		if (s[i] != c) {
			s[j++] = s[i];
		}
	}
	s[j] = s[i];
}

bool isFundamentalOperator(char c) {
	if (c == '+' || c == '-' || c == '*') {
		return true;
	}
	else {
		return false;
	}
}

void expressionAutoCorrector(char *s) {
	int i, j, k;
	char c;
	int flag = 0;
	for (i = 0, j = 0; c = s[i]; i++) {
		if (isNumber(c) || isOperator(c) || isVariable(c))
		{
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
	if (j < i-1) {
		flag = 1;
	}
	//duplicate ops like a++--**b will be regarded as a*b
	for (i = 0, j = 0; s[j]; i++,j++) {
		k = 1;
		while (!isStringEnd(*(s + j + k)) && isFundamentalOperator(*(s+j)) && isFundamentalOperator(*(s + j+k))) {
			k++;
			flag = 1;
		}
		j += k-1;
		s[i] = s[j];
	}
	s[i] = '\0';
	if (flag) {
		if(cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("This Expression is automatically corrected into:\n");
		
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		printf("%s\n", s);

		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("You can turn off corrector if you don't want it.\n");
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
		
}

//abc^2+abc*2(abc+a7x)^54abc