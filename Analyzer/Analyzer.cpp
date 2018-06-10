#include "Analyzer.h"
#include "io.h"
#include "sorts.h"
#include <math.h>
#define TORGB(x) ((x) / 255.0f)
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
	
	for(int i=0;i<3;i++) {
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
	an.registerFunction(selectsort, "Select");
	an.registerFunction(bubblesort, "Bubble");
	an.registerFunction(insertsort, "Insert", glm::vec3(TORGB(0x49), TORGB(0xbe), TORGB(0xa1)));
	an.registerFunction(mergesort, "Merge", glm::vec3(TORGB(0x50), TORGB(0x98), TORGB(0xd2)));//5094c9
	an.registerFunction(quicksort_simple, "Quick", glm::vec3(TORGB(0xae), TORGB(0xc5), TORGB(0x8d)));//#aec58d
	an.registerFunction(shellsort, "Shell", glm::vec3(TORGB(0x00), TORGB(0x71), TORGB(0xcc)));//#007acc
	
	an.registerFunction(radixsort_lsd, "Radix", glm::vec3(TORGB(0xa0), TORGB(0x60), TORGB(0xbd)));
	//an.registerFunction(countingsort, "Count");
	an.registerDataDealer(writeData);
	an.registerData(genArr);
	printf("##################################################################################\n"
		   "###                                    Analyzer                                ###\n"
		   "##################################################################################\n\n");
	printf("| Input 'exit' to stop\n");
	printf("| You can close the graph to re-enter numbers\n");
	printf("| To disable file operation, input 'io off'\n");
	while (1) {
		printf("\nInput the amount of data you want to test\n");
		printf("starting ending step (seperated by spaces)\n");
		fgets(buf, 1000, stdin);
		start = 0;
		end = 10000;
		step = 100;
		if (strcmp(buf, "exit\n") == 0) {
			break;
		}
		if (strcmp(buf, "io off\n")) {
			an.registerDataDealer(NULL);
			printf("file operation is disabled\n");
		}
		else if (strcmp(buf, "io on\n")) {
			an.registerDataDealer(writeData);
			printf("file operation is enabled\n");
		}
		else if (buf[0] == '\n' || buf[0] == '\r') {
			printf("We will use default settings.\n");
			printf("%d %d %d", start, end, step);
		}
		else {
			parseSetAmount(buf, start, end, step);
			printf("%d %d %d", start, end, step);
		}
		printf("\n");
		an.setDataAmount(start, end, step);
		an.benchmark();
	}
	return 0;
}