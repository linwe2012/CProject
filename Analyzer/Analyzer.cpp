#include "Analyzer.h"
#include "io.h"
#include "sorts.h"
#include <math.h>
//for test
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
void parseSetAmount(const char *s, int &start, int &end, int &step) {
	int num;
	
	for(int i=0;i<2;i++) {
		num = 0;
		while (*s == ' ')
			s++;
		while (*s >= '0' && *s <= '9') {
			num *= 10;
			num += *s - '0';
			s++;
		}
		if (i == 0) {
			start = num;
		}
		else if (i == 1) {
			end = num;
		}
		else {
			step = num;
		}
	}
}

int main()
{
	Analyzer<int> an;
	char buf[1000];
	int start, end, step;
	an.registerFunction(shellsort, "Shell");
	an.registerFunction(selectsort, "Select");
	an.registerFunction(bubblesort, "Bubble");
	an.registerFunction(insertsort, "Insert");
	an.registerFunction(mergesort, "Merge");
	an.registerFunction(quicksort_simple, "Quick");
	
	an.registerFunction(radixsort_lsd, "Radix");
	//an.registerFunction(countingsort, "Count");
	an.registerDataDealer(writeData);
	an.registerData(genArr);
	printf("##################################################################################\n"
		   "##                                    Analyzer                                  ##\n"
		   "##################################################################################\n");
	while (1) {
		printf("Input the amount of data you want to test\n");
		printf("starting ending step (seperated by single space)\n");
		fgets(buf, 1000, stdin);
		start = 0;
		end = 10000;
		step = 100;
		if (strcmp(buf, "exit") == 0) {
			break;
		}
		if (buf[0] == '\n' || buf[0] == '\r') {
			printf("We will use default settings.\n");
		}
		else {
			printf("%d %d %d", start, end, step);
			parseSetAmount(buf, start, end, step);
		}
		printf("\n");
		an.setDataAmount(start, end, step);
		an.benchmark();
	}
	return 0;
}