#pragma once
#include "io.h"
void countingsort(int *a, int count);

void radixsort_lsd(int *a, int count);

void quicksort(
	void *base, 
	int num, 
	int width,
	int(*cmp)(const void *, const void *) // function for compare if(a>b) return > 0, vice versa
);
//simplified & limited & recurse version of qsort
void quicksort_simple(int *a, int count);

//non-recurse version
void mergesort_bottomup(int *a, int count);
//recurse version
void mergesort(int *a, int count);

void shellsort(int *a, int count);

void selectsort(int a[], int length);
void bubblesort(int a[], int length);
void insertsort(int a[], int length);