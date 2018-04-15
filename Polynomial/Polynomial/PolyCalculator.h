#ifndef _POLYCALCULATOR_H_
#define _POLYCALCULATOR_H_
#include "ListsData.h"
/**
* @brief: add expression2 to expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/

void add(Expressions* &exp1, Expressions* &exp2);

/**
* @brief: subtract expression2 from expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/
void sub(Expressions* &exp1, Expressions* &exp2);

/**
* @brief: add expression2 to expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/
Expressions * mul(Expressions *exp1, Expressions *exp2);
void copyExp(Expressions* p1, Expressions* p2);

#endif // !_POLYCALCULATOR_H_

