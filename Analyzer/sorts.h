#pragma once
#include "io.h"
void countingsort(int *a, int count);
//aka Radix
void radixsort_lsd(int *a, int count);

//aka Qsort
void quicksort(
	void *base, 
	int num, 
	int width,
	int(*cmp)(const void *, const void *) // function for compare if(a>b) return > 0, vice versa
);
//for registration aka Qsort
void quicksort_rewrap(int *a, int count);
//simplified & limited & recurse version of qsort aka Quick
void quicksort_simple(int *a, int count);

//non-recurse version aka MgNR
void mergesort_bottomup(int *a, int count);
//recurse version  aka Merge
void mergesort(int *a, int count);
//aka Shell
void shellsort(int *a, int count);

void selectsort(int a[], int length);
void bubblesort(int a[], int length);
void insertsort(int a[], int length);