#include <stdlib.h>
#include <stdio.h>

#define LEFT 1
#define RIGHT 0



/*
struct SinglePoly
{
	PolyType coeff;
	PolyType degree;
	PolyVarType var;
};


struct Poly *head, pltail, prhead, prtail;

struct Poly *initPoly()
{
	var[0] = -1;
	
}

bool isNumber(char c)
{
	if (c <= '9' && c >= '0')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool isLetter(char c)
{
	if ((c <= 'z' && c >= 'a') || (c <= 'Z'&&c >= 'A')) {
		return true;
	}
	else
		return false;
}

bool isNewVar(PolyVarType v)
{
	int i = 0;
	while (var[i] != -1) {
		if (var[i] == v) {
			return false;
		}
		i++;
	}
	var[i] = v;
	var[i + 1] = -1;
	return true;
}

struct Poly * checkDuplicte(struct Poly *phead, struct Poly *checkee)
{
	int flag = 1;
	struct Poly *p1, *p2;
	while (flag) {
		if (phead->degree == checkee->degree) {
			p1 = phead->son;
			p2 = checkee->son;
			while (flag) {
				if (p1 == NULL && p2 == NULL) {
					return NULL;
				}
				else if (p1 == NULL || p2 == NULL) {
					return phead;
				}
				if (p1->coeff == p2->coeff) {
					p1 = p1->son;
					p2 = p2->son;
				}
				else return phead;
			}
		}
		if(flag != 0){
			phead = phead->next;
			if (phead == NULL) {
				return NULL;
			}
		}
	}
	
	
}

PolyType getNextNum(char *s, int *leap) {
	PolyType num = 0;
	PolyType denominator = 1;
	*leap = 0;
	while (*s != ' ') {
		s++;
		(*leap)++;
	}
		
	while (isNumber(*s) == true) {
		num = num * 10 + (*s) / denominator - '0';
		while (*++s != ' ')
			(*leap)++;
		if (*s == '.' || denominator > 1) {
			denominator = denominator * 10;
			while (*++s != ' ')
				(*leap)++;
		}
	}
	return num;
}

PolyType calculator(char *s)
{
	
}
*/
/*
void varStringSorter(char *Variables) {
	char *s = Variables;
	char sorted[]
	int i = 0, j;
	while (var[i] != -1 && i < MAXVAR) {
		s = Variables;
		while (*s) {

		}
	}
}

void fractionDisassembler(char *s, struct Poly *fractionArray[MAXVAR])
{
	int flag;
	int denominator = 1;
	
	if(*s == '(' )
	{
		*coeff = calculator(s);
	}
	else
	{
		if ((*s == '-')) {
			flag = -1;
			s++;
		}
		
	}
	while (*s != ' ')
		s++;
	while(isNumber(*s) == false && *s != '^'){
	
		if(isLetter(*s))
		{
			if (isNewVar(*s)) {

			}
		}
		else{
			printf("Unsupported Variable.\n");
			exit(0);
		}
		while (*s != ' ')
			s++;
	}
}



struct Poly *addPoly(char *s, int direct, struct Poly *ptail)
{
	PolyType coeff = 0, degree = 0, flag =1;
	struct Poly *ptr;
	

	ptr = (struct Poly *) malloc(sizeof(struct Poly));
	if (ptr == NULL) {
		printf("Fail to add a node.\n");
	}
	
	
	
	
	

}*/
