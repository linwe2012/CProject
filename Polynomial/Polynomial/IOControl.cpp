#include "IOControl.h"
static FILE *globe_fp;

char* freadExpression(char *buffer, char *address) {
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