#include "io.h"


int *genArr(int n)//输入n，生成n个0~65535的可重复随机正整数，保存到一个文本文档里，并返回生成数组的首地址 
{
	int *p;
	FILE *fp;
	int i, j, k;
	srand(time(NULL));
	p = (int *)malloc(n * sizeof(int));//根据所需数据规模大小分配空间 
	for (i = 1; i<n; i++) {
		*(p++) = rand() % (65535) + 1;//生成随机数 
	}
	*p = rand() % (65535) + 1;
	j = i; k = i;
	while (j>0) {
		 (p--); j--;
	}//在程序中显示 
	p++;
	if ((fp = fopen("number.txt", "w")) == NULL) {
		printf("File open error!\n"); getchar(); getchar();
		exit(0);
	}

	while (i>0) {
		fprintf(fp, "%d\n", *(p++));//写入文本文档 
		i--;
	}


	if (fclose(fp)) {
		printf("Cannot close the file!\n"); getchar(); getchar();
		exit(0);
	}

	while (k>0) {
		p--;
		k--;
	}
	return p;
}


void writeData(int *arr, int n, bool sorted, const char *name, double time)//读入一个整型数组的首地址以及数组元素个数，将他们保存到文本文档中 
{
	FILE *fp;
	int i;
	if (_access("record.txt", 0) == 0) {
		fopen_s(&fp, "record.txt", "a");
	}
	else {
		fopen_s(&fp, "record.txt", "w");
	}
	if (fp == NULL) {
		printf("File open error!\n"); getchar(); getchar();
		exit(0);
	}
	
	if (sorted) {
		fprintf(fp, "sorted by %s (time cost: %lf):\n", name, time);
	}
	else {
		fprintf(fp, "\n\n\nunsorted Array:\n");
	}
	for (i = 0; i<n; i++)
	{
		fprintf(fp, "%d\t", *(arr + i));//写入文本文档
	}
	fprintf(fp, "  __end\n");
	//printf("hi\n");
	if (fclose(fp)) {
		printf("Cannot close the file!\n"); getchar(); getchar();
		exit(0);
	}
}



int * readTXT(int n)//从一个文本文档中读取n个整型数据，返回该整型数组的首地址 
{
	FILE *fp;
	int i;
	int *p1;
	p1 = (int *)malloc(n * sizeof(int));//根据所需数据规模大小分配空间 

	if ((fp = fopen("number.txt", "r")) == NULL)//只读形式打开，准备读取数据 
	{
		printf("File open error!\n"); getchar(); getchar();
		exit(0);
	}

	for (i = 0; i<n; i++)
	{
		fscanf(fp, "%d", p1++);//写入数组 
	}

	if (fclose(fp)) {
		printf("Cannot close the file!\n"); getchar(); getchar();
		exit(0);
	}

	for (i = 0; i<n; i++)
	{
		p1--;//p1回到数组开头 
	}
	return p1;
}