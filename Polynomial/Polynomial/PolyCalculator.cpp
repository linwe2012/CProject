#include "PolyCalculator.h"
#include <stdlib.h>

//-1:p1 behind p2
//0: p1 = p2
//1:p1 before p2; p1 higher priority
int compare(Expressions* p1, Expressions* p2) {
	Poly*ptr1, *ptr2;
	int flag = 0;
	ptr1 = p1->son;
	ptr2 = p2->son;
	if (ptr1 == NULL && ptr2 == NULL)
		return 0;
	else if (ptr1 == NULL)
		return -1;
	else if (ptr2 == NULL)
		return 1;
	while (ptr1->degree == ptr2->degree && ptr1->var == ptr2->var) {
		if (ptr1->son == NULL && ptr2->son == NULL)
			return 0;
		else if (ptr1->son == NULL)
			return -1;
		else if (ptr2->son == NULL)
			return 1;
		else {
			ptr1 = ptr1->son;
			ptr2 = ptr2->son;
		}
	}
	if (ptr1->var != ptr2->var) {
		flag = ptr2->var - ptr1->var;
	}
	else {
		flag = ptr1->degree - ptr2->degree;
	}
	return flag;
}
/*
void copyExp(Expressions* p1, Expressions* p2) {
	Poly*ptr1, *ptr2, *p;
	p = (Poly*)malloc(sizeof(Poly));
	p2->son = p;
	p2->coeff = p1->coeff;
	ptr1 = p1->son;
	ptr2 = p2->son;
	while (ptr1 != NULL) {
		ptr2->degree = ptr1->degree;
		ptr2->var = ptr1->var;
		p = (Poly*)malloc(sizeof(Poly));
		p->son = NULL;
		ptr2->son = p;
		ptr2 = ptr2->son;
		ptr1 = ptr1->son;
	}
}
//copy from p1 to p2;
Expressions* findtail(Expressions* exp1) {
	Expressions* tail = exp1;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	return tail;
}
*/

void add(Expressions* &exp1, Expressions* &exp2)
{
	Expressions*p1, *p2, *exp1_next = NULL;
	int temp;
	Expressions *exp1_dup = exp1;

	if (exp1 && exp2 && compare(exp2, exp1) > 0) {
		p1 = exp1;
		exp1 = exp2;
		exp2 = p1;
		exp1_dup = exp1;
	}
	else {
		while (exp1_dup && exp2 && compare(exp2, exp1_dup) == 0) {
			exp1_dup->coeff += exp2->coeff;
			p2 = exp2;
			exp2 = exp2->next;
			freePolyList((p2->son));
			free(p2);
		}
		//printf("start¡® exp1 = exp2  ");
	}
	p1 = exp1;
	for (;exp1_dup != NULL && exp1_dup->next && exp2;) {
		exp1_next = exp1_dup->next;
		//printf("for loop ");
		while (exp2 && (temp = compare(exp2, exp1_next)) >= 0) //exp2 has higher or equal priority
		{
			//printf("while loop ");
			//same: merge & free
			if (temp == 0) {
				exp1_next->coeff += exp2->coeff;
				p2 = exp2;
				exp2 = exp2->next;
				freePolyList((p2->son));
				free(p2);
				//printf("exp1 = exp2  ");
			}
			//different: absorb into exp1
			else {
				exp1_dup->next = exp2;
				exp1_dup = exp1_dup->next;
				exp2 = exp2->next;
				//printf("absorb  ");
			}
		}
		//exp1.next redirect to original next
		exp1_dup->next = exp1_next;
		if (p1 == exp1_dup) {
			exp1_dup = exp1_dup->next;
		}
		p1 = exp1_dup;
	}
	if (exp2 != NULL) {
		exp1_dup->next = exp2;
	}
}

void sub(Expressions* &exp1, Expressions* &exp2)
{
	Expressions *exp = exp2;
	while (exp) {
		exp->coeff = -exp->coeff;
		exp = exp->next;
	}
	add(exp1, exp2);
}
/*
void add(Expressions* exp1, Expressions* exp2) {
	Expressions*p1, *p2, *p, *tail;
	
	tail = findtail(exp1);
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		
		for (p1 = exp1; p1 != NULL; p1 = p1->next) {
			if (compare(p1, p2) >) {
				p1->coeff += p2->coeff;
				p2->coeff = 0;

				break;
			}
			p = p1;
		}
	}
	
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		if (p2->coeff != 0) {
			p = (Expressions *)malloc(ExpressionSize);
			copyExp(p2, p);
			tail->next = p;
			tail = tail->next;
		}
	}
}*/
	//add exp2 to exp1;

int addVariableToExpression(PolyType degree, PolyVarType var,Expressions *exp) {

	int flag = 0;
	Poly *newVarNode, *ptr;
	Poly *head = exp->son;

	if (head == NULL) {
		newVarNode = (Poly *)malloc(PolySize);
		newVarNode->degree = degree;
		newVarNode->son = NULL;
		newVarNode->var = var;
		exp->son = newVarNode;
	}
	else if (head->var > var) {
		newVarNode = (Poly *)malloc(PolySize);
		newVarNode->degree = degree;
		newVarNode->var = var;
		newVarNode->son = head;
		exp->son = newVarNode;
	}
	else if (head->var == var) {
		head->degree += degree;
	}
	else {
		ptr = head->son;
		while (ptr) {
			if (head->var == var) {
				head->degree += degree;
				flag = 1;
				break;
			}
			else if (ptr->var > var && head->var < var) {
				newVarNode = (Poly *)malloc(PolySize);
				newVarNode->degree = degree;
				newVarNode->var = var;
				newVarNode->son = ptr;
				head->son = newVarNode;
				flag = 1;
				break;
			}
			else {
				head = ptr;
				ptr = ptr->son;
			}
		}
		if (head->var == var) {
			head->degree += degree;
		}
		else if (flag == 0) {
			newVarNode = (Poly *)malloc(PolySize);
			newVarNode->degree = degree;
			newVarNode->son = NULL;
			newVarNode->var = var;
			head->son = newVarNode;
		}
	}

	return 0;
}

Expressions * mul(Expressions *exp1, Expressions *exp2)
{
	int size1 = sizeof(Expressions);
	//define size of structs

	Expressions *exp3 = NULL, *p1, *p2, *p3, *head, *temp_head;
	Poly *ptr1, *ptr2; //*ptr3, *head, *tail;
	//definition of points
	p3 = NULL;
	head = NULL;
	for (p1 = exp1; p1 != NULL; p1 = p1->next) {
		temp_head = exp3 = NULL;
		for (p2 = exp2; p2 != NULL; p2 = p2->next) {
			p3 = (Expressions *)malloc(ExpressionSize);
			//create new space for new list			
			p3->coeff = p1->coeff*p2->coeff;
			p3->next = NULL;
			p3->son = NULL;

			ptr1 = p1->son;
			ptr2 = p2->son;
			while (ptr1) {
				addVariableToExpression(ptr1->degree, ptr1->var, p3);
				ptr1 = ptr1->son;
			}
			while (ptr2) {
				addVariableToExpression(ptr2->degree, ptr2->var, p3);
				ptr2 = ptr2->son;
			}
			if (exp3 == NULL)
			{
				temp_head = exp3 = p3;
			}
			else
			{
				exp3->next = p3;
				exp3 = p3;
			}
			
		}
		if (head == NULL)
			head = temp_head;
		else
			add(head, temp_head);
	}
	return head;
}

/*
void sub(Expressions* exp1, Expressions* exp2) {
	Expressions*p1, *p2, *p, *tail;
	tail = findtail(exp1);

	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		for (p1 = exp1; p1 != NULL; p1 = p1->next) {
			if (compare(p1, p2)) {
				p1->coeff -= p2->coeff;
				p2->coeff = 0;
				break;
			}
		}
	}
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		if (p2->coeff != 0) {
			p2->coeff = -p2->coeff;
			p = (Expressions *)malloc(ExpressionSize);
			copyExp(p2, p);
			tail->next = p;
			tail = tail->next;
		}
	}
}
*/