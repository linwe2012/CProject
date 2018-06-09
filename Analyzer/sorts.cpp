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

//void radixsort_msd(int *a, int count)
//{
//	int max = getMax(a, count);
//	int cnt[BUCKET_SIZE] = { 0 };
//	int *bucket = (int *)malloc(sizeof(int) * count);
//	int digitcnt = 0;
//	int i, j;
//	int power = 1;
//	while (max > 0) {
//		max /= 10;
//		digitcnt++;
//		power *= 10;
//	}
//	power /= 10;
//	for (i = 0; i < digitcnt; i++, power /= 10) {
//		memset(cnt, 0, sizeof(int)*BUCKET_SIZE);
//		for (j = 0; j < count; j++) {
//			cnt[getDigit(a[j], power)] ++;
//		}
//		for (j = 1; j < BUCKET_SIZE; j++) {
//			cnt[j] = cnt[j] + cnt[j - 1];
//		}
//		for (j = count - 1; j >= 0; j--) {
//			int d = getDigit(a[j], power);
//			bucket[cnt[d] - 1] = a[j];
//			cnt[d] --;
//		}
//		memcpy(a, bucket, sizeof(int)*count);
//		for (j = 1; j < BUCKET_SIZE; j++) {
//			cnt[j] = cnt[j] + cnt[j - 1];
//		}
//	}
//}

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
int main()
{
	int a[20];
	srand(time(NULL));
	for (int k = 0; k < 20; k++) {
		a[k] = rand() % 0xffff;
	}
	for (int k = 0; k < 20; k++) {
		printf("%d\t", a[k]);
	}
	printf("\n");
	//radixsort_lsd(a, 20);
	quicksort(a, 20, sizeof(int), quick_cmp);
	//qsort(a, 20, sizeof(int), quick_cmp);
	for(int k = 0; k < 20; k++) {
		printf("%d\t", a[k]);
	}
	getchar(); getchar();
	return 0;
}