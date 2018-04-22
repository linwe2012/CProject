#pragma once
#include <csignal>
#include <iostream>
#include <csetjmp>
#include <ctime>
#include "ExpressionSet.h"
#include "PolishReverse.h"

extern std::jmp_buf jmp_to_begin;

void setSignalHandler();

void cleanUpHalfTime(bool ifRegister = false, 
	Expressions **exp1 = NULL, 
	Expressions **exp2 = NULL, 
	Expressions **exp_temp =NULL, 
	Expressions **exp3 =NULL, 
	ExpressionSets **expStack = NULL);

void cleanUp(bool ifRegister = false, 
	OperatorStack *op = NULL, 
	FragmentStack *frag = NULL);