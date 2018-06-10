#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include <math.h>
int getMax(int *a, int count)
{
	int max = a[0];
	for (int i = 1; i < count; i++) {
		if (max < a[i])
			max = a[i];
	}
	return max;
}

inline void swapint(int *a, int *b)
{
	int c = *b;
	*b = *a;
	*a = c;
}
/////////////////////////////////////
////     Counting Sort           ////
/////////////////////////////////////
void countingsort(int *a, int count)
{
	int *idx;
	int i, j, max;
	
	max = getMax(a, count);
	idx = (int *)malloc((max+1) * sizeof(int));
	if (idx == NULL) {
		printf("countingsort::Fails to allocate memory\n");
		return;
	}
	memset(idx, 0, max * sizeof(int));
	for (i = 0; i < count; i++) {
		idx[a[i]] ++;
	}
	for (i = 0, j = 0; i <= max; i++)
	{
		while (idx[i] > 0) {
			a[j++] = i;
			idx[i]--;
		}
	}
	free(idx);
}

/////////////////////////////////////
////        Radix Sort           ////
/////////////////////////////////////
#define BUCKET_SIZE 10
struct buckets {
	int value;
	buckets *son[BUCKET_SIZE];
};
inline int getDigit(int a, int power) 
{
	a /= power * 10;
	a %= 10;
	return a;
}
void radixsort_lsd(int *a, int count)
{
	int max = getMax(a, count);
	int cnt[BUCKET_SIZE] = { 0 };
	int *bucket = (int *)malloc(sizeof(int) * count);
	int digitcnt = 0;
	int i, j;
	int power;
	while (max > 0){
		max /= 10;
		digitcnt++;
	}
	for (i = 0, power = 1; i < digitcnt; i++, power *= 10) {
		memset(cnt, 0, sizeof(int)*BUCKET_SIZE);
		for (j = 0; j < count; j++) {
			cnt[getDigit(a[j], power)] ++;
		}
		for (j = 1; j < BUCKET_SIZE; j++) {
			cnt[j] = cnt[j] + cnt[j - 1];
		}
		for (j = count - 1; j >= 0; j--) {
			int d = getDigit(a[j], power);
			bucket[cnt[d] - 1] = a[j];
			cnt[d] --;
		}
		memcpy(a, bucket, sizeof(int)*count);
	}
}

/////////////////////////////////////
////        Quick Sort           ////
/////////////////////////////////////
#define STACKSZ 0x20
void swap(char *a, char *b, int width)
{
	char temp;
	if (a != b) 
		while (width--) {
			temp = *a;
			*a++ = *b;
			*b++ = temp;
		}
}

void quicksort(void *base, int num, int width, int(*cmp)(const void *, const void *))
{
	char *lo, *hi;
	char *mid;
	char *loguy, *higuy; 
	char *histack[STACKSZ];
	char *lostack[STACKSZ];
	int stkptr;
	int size;

	lo = (char *)base;
	hi = (char *)base + (num - 1) * width;
	stkptr = 0;
	
recurse:
	size = (hi - lo) / width + 1;
	if (hi - lo == width) {
		if (cmp(lo, hi) > 0) {
			swap(lo, hi, width);
		}
	}
	else {
		mid = lo + (size / 2) * width;
		//mid < lo < high
		if (cmp(mid, lo) > 0) {
			swap(mid, lo, width);
		}
		if (cmp(mid, hi) > 0) {
			swap(mid, hi, width);
		}
		if (cmp(lo, hi) > 0) {
			swap(lo, hi, width);
		}
		//traveling pointers
		loguy = lo;
		higuy = hi;
		for (; loguy < higuy;) {
			do {
				higuy -= width;
			} while (loguy < higuy && cmp(higuy, lo) > 0);
			do {
				loguy += width;
			} while (loguy < higuy && cmp(loguy, lo) <= 0);
			if (loguy < higuy) {
				swap(loguy, higuy, width);
			}
		}
		//here higuy must be the middle one
		swap(higuy, lo, width);

		loguy = higuy + width;
		higuy -= width;
		//make sure the smaller one sort first
		if (higuy - lo >= hi - loguy)
		{
			if (lo < higuy) {
				histack[stkptr] = higuy;
				lostack[stkptr] = lo;
				stkptr++;
			}
			if (loguy < hi) {
				lo = loguy;
				goto recurse;
			}
		}
		else {
			if (loguy < hi) {
				histack[stkptr] = hi;
				lostack[stkptr] = loguy;
				stkptr++;
			}
			if (lo < higuy) {
				hi = higuy;
				goto recurse;
			}
		}
	}
	//clear stack
	--stkptr;
	if (stkptr >= 0) {
		lo = lostack[stkptr];
		hi = histack[stkptr];
		goto recurse;
	}
	else {
		return;
	}
}
int quick_cmp(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

void quicksort_simple(int *a, int count) 
{
	int i, j, swap, temp;
	if (count < 2) return;
	temp = *a;
	i = 0; j = count - 1;

	while (i != j) {
		while (a[j] >= temp && i<j)
			j--;    
		while (a[i] <= temp && i<j)
			i++;

		if (i<j) {
			swap = a[i];
			a[i] = a[j];
			a[j] = swap;
		}
	}
	*a = a[i];
	a[i] = temp;
	quicksort_simple(a, i);
	quicksort_simple(a + i+1, count - i-1);
}

/////////////////////////////////////
////        Merge Sort           ////
/////////////////////////////////////
void merge(int *from, int *to, int lo, int hi, int mid) {
	int p = lo;
	int q = mid+1;
	for (int i = lo; i <= hi; i++) {
		if (p > mid && q <= hi) { to[i] = from[q++];     }
		else if (q > hi) { to[i] = from[p++];            }
		else if (from[p] < from[q]) { to[i] = from[p++]; }
		else to[i] = from[q++];
	}
}
inline int minint(int a, int b) {
	return (a < b) ? a : b;
}
//non-recurse version
void mergesort_bottomup(int *a, int count)
{
	int *orgn = a;
	int *dst = (int *)malloc(sizeof(int)*count);
	int *tmp;
	int split = 2;
	int lo, hi;
	int cnt = 0;
	if (dst == NULL) {
		printf("mergesort_bottomup::Fails to allocate memory\n");
		return;
	}
	for (split = 2; split/2 < count; split *= 2) {
		for (lo = 0; lo < count; lo += split) {
			hi = minint(lo + split - 1, count - 1);
			merge(orgn, dst, lo, hi, lo + split / 2 - 1);
		}
		tmp = orgn;
		orgn = dst;
		dst = tmp;
		cnt++;
	}
	if (cnt % 2 == 0 && cnt != 0) {
		free(dst);
	}
	else if(cnt != 0) {
		memcpy(a, orgn, sizeof(int)*count);
		free(orgn);
	}
}

//recurse version
void mergesort(int *a, int count)
{
	int *res;
	if (count > 1) {
		mergesort(a, count / 2);
		mergesort(a + count / 2, count - count / 2);
		res = (int *)malloc(sizeof(int)*count);
		merge(a, res, 0, count - 1, count / 2 - 1);
		memcpy(a, res, sizeof(int)*count);
		free(res);
	}
}

/////////////////////////////////////
////        Shell Sort           ////
/////////////////////////////////////
void shellsort(int *a, int count) 
{
	int h = 1;
	int i, j;
	int swap;
	//magic
	while (h < count / 3) h = h * 3 + 1;
	while (h > 0) {
		for (i = h; i < count; i++) {
			for (j = i; j >= h && a[j] < a[j - h]; j -= h) {
				swap = a[j];
				a[j] = a[j - h];
				a[j - h] = swap;
			}
		}
		h /= 3;
	}
}

void heapsort(int *a, int count)
{

}

void selectsort(int a[], int length)//利用选择排序法对数组进行排序，并记录时间频度与时间复杂度 
{
	int i, j;
	int k;
	int temp;
	clock_t start, end;
	for (i = 0; i<length - 1; i++)
	{
		k = i;
		for (j = i + 1; j<length; j++)
		{
			if (a[j]<a[k])
			{
				k = j;
			}
		}
		if (i != k)
		{
			temp = a[i];
			a[i] = a[k];
			a[k] = temp;
		}
	}
}

void bubblesort(int a[], int length)//利用冒泡排序法对数组进行排序，并记录时间频度与时间复杂度
{
	int i, j;
	int temp;
	for (i = 0; i<length - 1; i++)
	{
		for (j = 0; j<length - 1; j++)
		{
			if (a[j]>a[j + 1])
			{
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}
void insertsort(int a[], int length)   //利用插入排序法对数组进行排序，并记录时间频度与时间复杂度
{
	int i, j;
	int temp;
	clock_t start, end;
	int count = 1;//时间频度 
	start = clock();//计时开始 
	for (i = 1; i<length; i++)
	{
		temp = a[i];
		j = i - 1;
		while (j >= 0 && temp<a[j])
		{
			a[j + 1] = a[j];
			j--;
			count += 2;
		}
		a[j + 1] = temp;//找到合适位置，将元素插入。
		count += 3;
	}
}
/*
#define AMOUNT 20
int main()
{
	int a[AMOUNT];
	srand(time(NULL));
	for (int k = 0; k < AMOUNT; k++) {
		a[k] = rand() % 0xff;
	}
	for (int k = 0; k < AMOUNT; k++) {
		printf("%d\t", a[k]);
	}
	printf("\n");
	//a[0] = 1; a[1] = 3; a[2] = 2; a[3] = 4;
	//a[0] = 3; a[1] = 1; a[2] = 2;
	//quicksort(a, 20, sizeof(int), quick_cmp);
	//mergesort_bottomup(a, AMOUNT);
	//mergesort(a, AMOUNT);
	//shellsort(a, AMOUNT);
	quicksort_simple(a, AMOUNT);
	for(int k = 0; k < AMOUNT; k++) {
		printf("%d\t", a[k]);
	}
	getchar(); getchar();
	return 0;
}*/