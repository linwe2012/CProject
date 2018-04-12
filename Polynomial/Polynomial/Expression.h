#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

typedef int PolyType;
typedef int PolyVarType;

typedef struct exp_exp
{
	PolyType coeff;
	struct exp_exp *next;
	struct poly_poly *son;
} Expressions;
#endif // !_EXPRESSION_H_

