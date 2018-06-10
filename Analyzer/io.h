#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include  <io.h>

int *genArr(int n);
void writeData(int *arr, int n, bool sorted, const char *name, double time);
int * readTXT(int n);