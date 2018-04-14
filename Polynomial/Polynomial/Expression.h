#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
#define MAXVAR 26

/*可以更改typedef 为double来支持小数*/
typedef int PolyType;
/*Note that PolyVarType is an integer denoting its column in the varible table.
* In order to sort variables, one just have to compare its value.
*/
typedef int PolyVarType;

typedef struct poly_poly
{
	PolyType degree;
	PolyVarType var;
	struct poly_poly *son;
} Poly;

typedef struct exp_exp
{
	PolyType coeff;
	struct exp_exp *next;
	struct poly_poly *son;
} Expressions;

extern int ExpressionSize;
extern int PolySize;

extern char varTable[MAXVAR];

void freePolyList(Poly *head);
void freeExpression(Expressions *head);
int addNewVariable(char var, Expressions *exp);

#endif // !_EXPRESSION_H_

