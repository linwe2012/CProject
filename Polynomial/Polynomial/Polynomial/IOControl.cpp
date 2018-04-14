#include "IOControl.h"
#include "PolishReverse.h"
#include <windows.h>
#include "command.h"

static FILE *globe_fp;
bool isFundamentalOperator(char c);

char* fgetExpression(char *buffer, char *address) 
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
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("freadExpression::Fails to open file\n"
			"Address: %s, exAdress, %s,  offset: %ld\n", address, exAddress,  offset);
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
	fseek(fp, offset, SEEK_SET);
	while ((temp_c = fgetc(fp)) != '#' && temp_c != EOF) 
		;
	if (feof(fp)) {
		if (fclose(fp)) {
			throwError("freadExpression::Can't close file at ", GREY);
			printf("%s\n", address);
		}
		return NULL;
	}
	else {
		
		if ((temp_c = fgetc(fp)) == '\n') {
			name[0] = '\0';
		}
		else
		{
			name[0] = temp_c;
			fgets(name + 1, MAX_NAME - 1, fp);
		}
		
		while ((temp_c = fgetc(fp)) != '$' && temp_c != EOF)
			;
		fgets(buffer, MAX_BUFFER, fp);

		if (fclose(fp)) {
			throwError("freadExpression::Can't close file at ", GREY);
			printf("%s\n", address);
		}
		return name;
	}
	return NULL;
}
ExpressionSets *freadExpression(int ID, const char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps = NULL;
	fopen_s(&fp, FileAddress, "rb");
	if (fp == NULL) {
		throwError("freadExpression::Fails to open file at ", GREY);
		puts(FileAddress);
	}
	fread(exps, expressionSetsSize, 1, fp);
	while (exps != NULL && exps->ID != ID && !feof(fp)) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (feof(fp)) {
		throwError("freadExpression::Can't find expression whose id is", GREY);
		printf("%d\n", ID);
		exps = NULL;
	}
	if (fclose(fp)) {
		throwError("freadExpression::Can't close file at ", GREY);
		printf("%s\n", FileAddress);
	}
	return exps;
}

ExpressionSets *freadExpression_name(const char *exp_name, char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps = NULL;
	fopen_s(&fp, FileAddress, "rb");
	if (fp == NULL) {
		throwError("freadExpression_name::Fails to open file at ", GREY);
		puts(FileAddress);
	}
	fread(exps, expressionSetsSize, 1, fp);
	while (strcmp(exp_name, exps->name) != 0 && !feof(fp)) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (strcmp(exp_name, exps->name) != 0)
	{
		throwError("freadExpression_name::Can't find expression whose name is", GREY);
		printf("%s\n", exp_name);
		exps = NULL;
	}
	if (fclose(fp)) {
		throwError("freadExpression_name::Can't close file at ", GREY);
		puts(FileAddress);
	}
	return exps;
}

void fwriteExpression(ExpressionSets *exps, char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps1 = NULL;
	fopen_s(&fp, FileAddress, "rb+");
	if (fp == NULL) {
		fopen_s(&fp, FileAddress, "wb");
		if (fclose(fp)) {
			throwError("fwriteExpression::Can't close file at ", GREY);
			puts(FileAddress);
		}
		fopen_s(&fp, FileAddress, "rb+");
		if (fp == NULL) {
			throwError("fwriteExpression::Fails to open file at ", GREY);
			puts(FileAddress);
		}
	}
	fread(exps, expressionSetsSize, 1, fp);
	while (!feof(fp) && strcmp(exps1->name, exps->name) != 0) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (!feof(fp)) {
		throwError("fwriteExpression::Expression whose name duplicated\n", GREY);
		printf("Input 0 to overwrite it. Input others to abort saving.\n");
		if (getchar() != '0') {
			fflush(stdin);
			if (fclose(fp)) {
				throwError("fwriteExpression::Can't close file at ", GREY);
				puts(FileAddress);
			}
			return;
		}
		else {
			fwrite(exps, expressionSetsSize, 1, fp);
			if (fclose(fp)) {
				throwError("fwriteExpression::Can't close file at ", GREY);
				puts(FileAddress);
			}
			return;
		}
		fflush(stdin);
	}
	fseek(fp, 0, SEEK_END);
	fwrite(exps, expressionSetsSize, 1, fp);
	if (fclose(fp)) {
		throwError("fwriteExpression::Can't close file at ", GREY);
		puts(FileAddress);
	}
	return;
}


int stringLegalchecker(char*s) 
{
	int legal = 0;
	int errors = 0;
	char c;
	int i;
	char *head;
	head = s;

	c = *s;
	if (isOperator(c)) {
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("stringLegalchecker::The first character: ");
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("%c ", c);
		throwError("is not a number\n", GREY);
		errors++;
		legal = -1;
		if (errors > cmd_maxErrorLog) {
			return -1;
		}
	}
	while (!isStringEnd(c)) {
		if (isNumber(c) || isOperator(c) || isVariable(c)) {
			if (c == '^' && !isStringEnd(*(s + 1)) ) {
				i = 1;
				//a^-abc
				if (*(s + 1) == '-') {
					if (!isStringEnd(*(s + 2)) && isVariable(*(s + 2))) {
						throwError("stringLegalchecker::Index has variable\n", GREY);
						errors++;
						legal = -1;
						if (errors > cmd_maxErrorLog) {
							return -1;
						}
					}
				}
				else if (isVariable(*(s + 1))) {
					throwError("stringLegalchecker::Index has variable\n", GREY);
					errors++;
					legal = -1;
					if (errors > cmd_maxErrorLog) {
						return -1;
					}
				}
			}
		}
		else if (c == ' ' && legal == 0) {
			legal = 1;
		}
		else {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
			printf("stringLegalchecker::unknow character ");
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			printf("%c\n", c);
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			errors++;
			legal = -1;
			if (errors > cmd_maxErrorLog) {
				return -1;
			}
		}
		if (isFundamentalOperator(*s) && isFundamentalOperator(*(s + 1)) &&
			!isStringEnd(*(s + 1)) && !isStringEnd(*(s + 2)) && !(*(s+1) == '-') && isNumber(*(s + 2))) {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
			printf("stringLegalchecker::Dupilicated binary operaters: ");
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			printf("%c%c\n", *s, *(s+1));
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			errors++;
			legal = -1;
			if (errors > cmd_maxErrorLog) {
				return -1;
			}
		}
		c = *++s;
	}
	
	while (isFundamentalOperator(*s) && s > head) {
		s--;
		throwError("stringLegalchecker::Binary operator at the end of expression\n", GREY);
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
	s[j] = '\0';
}

bool isFundamentalOperator(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '^') {
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
		if (s[i] == '(' && s[i + 1] == ')') {
			i = i + 1;
			continue;
		}
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
		if (*(s + j + k -1) == '-' && isNumber(*(s + j + k)) && k>1) {
			s[i++] = s[j + k - 2];
		}
		j += k-1;
		s[i] = s[j];
	}
	s[i] = '\0';
	for (i = 0; s[i+1]; i++) {
		if (s[i] == '^' && isVariable(s[i + 1])) {
			flag = 1;
			s[i] = '*';
		}
	}
	while (isFundamentalOperator(s[i])) {
		s[i] = '\0';
		i--;
	}
	if (flag) {
		throwError("This Expression is automatically corrected into:\n", GREY);
		printf("%s\n", s);
		throwError("You can turn off corrector if you don't want it.\n", GREY);
	}
		
}

int parentheseAutoAdder(char *s, int num, int maxBuffer) 
{
	int offset, count, i;
	bool isOp;
	int flag = 0;
	char *end = s;
	offset = 0;
	while (*end != '\0') {
		end++;
	}
	count = end - s + 1;
	if (isOp = isOperator(*s) || num < 0) {
		if (isOp) {
			count++;
			offset++;
			
		}
		if (num < 0) {
			count -= num;
			offset -= num;
		}
		if (count > maxBuffer) {
			throwError("parentheseAutoAdder::Exceeds max buffer\n", GREY);
			return -1;
		}
		else {
			for (i = count - 1; i >= offset; i--) {
				s[i] = s[i - 1];
			}
			if (isOp) {
				s[i--] = '1';
			}
			for (; i >= 0; i--) {
				s[i] = '(';
			}
		}
		flag = 1;
	}
	if (num > 0) {
		if (count + num > maxBuffer) {
			throwError("parentheseAutoAdder::Exceeds max buffer\n", GREY);
			return -1;
		}
		for (i = num; i > 0; i--) {
			s[count + i - 2] = ')';
		}
		flag = 1;
	}
	return flag;
}

//abc^2+abc*2(abc+a7x)^54abc