#include <stdlib.h>
#include <stdio.h>
#define MAXVAR 26
#define LEFT 1
#define RIGHT 0
typedef int PolyType;
typedef char PolyVarType;

struct Poly
{
	PolyType coeff;
	PolyType degree;
	PolyVarType var;
	struct Poly *next;
	struct Poly *son;
};

PolyVarType var[MAXVAR];
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
		phead = phead->next;
		if (phead == NULL) {
			return NULL;
		}
	}
}

struct Poly *addPoly(char *s, int direct, struct Poly *ptail)
{
	PolyType coeff = 0, degree = 0, flag =1;
	struct Poly *ptr;
	

	ptr = (struct Poly *) malloc(sizeof(struct Poly));
	if (ptr == NULL) {
		printf("Fail to add a node.\n")
	}
	while (*s != ' ')
		s++;
	if ((*s == '-')) {
		flag = -1;
		s++;
	}
	while(isNumber(*s) == true ){
		coeff = coeff * 10 + *s - '0';
		while (*++s != ' ')
			;
	}
	while (*s != ' ')
		s++;
	

}