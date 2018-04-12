#ifndef _LISTSDATA_H_
#define _LISTSDATA_H_
#include "Expression.h"

/*可以更改typedef 为double来支持小数*/

#define MAXVAR 26

/*Note that PolyVarType is an integer denoting its column in the varible table.
* In order to sort variables, one just have to compare its value.
*/
typedef struct poly_poly
{
	PolyType degree;
	PolyVarType var;
	struct poly_poly *son;
} Poly;



PolyVarType varTable[MAXVAR];

#endif
