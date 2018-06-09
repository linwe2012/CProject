#include "Analyzer.h"
#include "io.h"
#include <math.h>

void func(int *a, int count) {
	int i = 0;
	while (i < count*pow(1.3, count)) {
		*(a + int(i / pow(1.3, count))) = 1;
		i++;
	}
}

void funl(int *a, int count)
{
	int i = 0;
	while (i < count*0xfffff) {
		*(a + i/0xfffff) = 1;
		i++;
	}
}


int *gen(int c) {
	int *a = (int *)malloc(c * sizeof(int));
	return a;
}

void selectsort(int a[], int length);
void bubblesort(int a[], int length);
void insertsort(int a[], int length);

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
int main()
{
	Analyzer<int> an;
	//an.registerFunction(func, "fuck");
	//an.registerFunction(funl, "linear");
	int n;
	int *array;
	int *a;
	an.registerFunction(selectsort, "Select");
	an.registerFunction(bubblesort, "Bubble");
	an.registerFunction(insertsort, "Insert");
	an.registerData(genArr);
	an.benchmark();
}