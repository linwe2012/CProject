#include "Analyzer.h"
#include <math.h>

void func(int *a, int count) {
	int i = 0;
	while (i < count*pow(1.5, count)) {
		*(a + int(i / pow(2, count))) = 1;
		i++;
	}
}

int *gen(int c) {
	int *a = (int *)malloc(c * sizeof(int));
	return a;
}
int main()
{
	Analyzer<int> an;
	an.registerFunction(func, "fuck");
	an.registerData(gen);
	an.benchmark();
	while (1);
}