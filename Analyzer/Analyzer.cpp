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

int cmdParse(char *buf, Analyzer<int> &an) {
#ifdef _WIN32
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_GREY);
#endif // _WIN32
	
	int flag = 1;
	if (strcmp(buf, "io off\n") == 0) {
		an.registerDataDealer(NULL);
		printf("file operation is disabled\n");
		flag = 0;
	}
	else if (strcmp(buf, "io on\n") == 0) {
		an.registerDataDealer(writeData);
		printf("file operation is enabled\n");
		flag = 0;
	}
	else if (strcmp(buf, "cancel bubble\n") == 0 || strcmp(buf, "cancel Bubble\n") == 0) {
		if (an.cancelFunction("Bubble") == 0) {
			printf("Bubble Sort Cancelled\n");
		}
		else {
			printf("Can't Find Bubble Sort\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel select\n") == 0 || strcmp(buf, "cancel Select\n") == 0) {
		if (an.cancelFunction("Select") == 0) {
			printf("Selection Sort Cancelled\n");
		}
		else {
			printf("Can't Find Selection Sort\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel insert\n") == 0 || strcmp(buf, "cancel Insert\n") == 0) {
		if (an.cancelFunction("Insert") == 0) {
			printf("Insertion Sort Cancelled\n");
		}
		else {
			printf("Can't Find Insertion Sort\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel merge\n") == 0 || strcmp(buf, "cancel Merge\n") == 0) {
		if (an.cancelFunction("Merge") == 0) {
			printf("Merge Sort Cancelled\n");
		}
		else {
			printf("Can't Find Merge Sort\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel quick\n") == 0 || strcmp(buf, "cancel Quick\n") == 0) {
		if (an.cancelFunction("Quick") == 0) {
			printf("Quick Sort Cancelled\n");
		}
		else {
			printf("Can't Find Quick Sort\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel 1sort\n") == 0 || strcmp(buf, "cancel Qsort\n") == 0) {
		if (an.cancelFunction("Qsort") == 0) {
			printf("Quick Sort (Non Recursive version) Cancelled\n");
		}
		else {
			printf("Can't Find Quick Sort(Non Recursive version)\n");
		}
		flag = 0;
	}
	else if (strcmp(buf, "cancel radix\n") == 0 || strcmp(buf, "cancel Radix\n") == 0) {
		if (an.cancelFunction("Radix") == 0) {
			printf("Radix Sort Cancelled\n");
		}
		else {
			printf("Can't Find Radix Sort\n");
		}
		flag = 0;
	}
	fflush(stdin);
#ifdef _WIN32
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
#endif // _WIN32
	return flag;
}
#define OUTPUT_INSTRUCTION 0x1

//VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
//{
//	if (lpParam == NULL)
//	{
//		printf("TimerRoutine lpParam is NULL\n");
//	}
//	else
//	{
//		// lpParam points to the argument; in this case it is an int
//
//		printf("Timer routine called. Parameter is %d.\n",
//			*(int*)lpParam);
//		if (TimerOrWaitFired)
//		{
//			printf("The wait timed out.\n");
//		}
//		else
//		{
//			printf("The wait event was signaled.\n");
//		}
//	}
//}

int main()
{
	Analyzer<int> an;
	char buf[1000];
	int start, end, step;
	int output[10] = {0};
	
	an.registerFunction(selectsort, "Select");
	an.registerFunction(bubblesort, "Bubble");
	an.registerFunction(insertsort, "Insert", glm::vec3(TORGB(0x49), TORGB(0xbe), TORGB(0xa1)));
	an.registerFunction(mergesort, "Merge", glm::vec3(TORGB(0x50), TORGB(0x98), TORGB(0xd2)));//5094c9
	an.registerFunction(mergesort_bottomup, "MgNR");
	an.registerFunction(quicksort_simple, "Quick", glm::vec3(TORGB(0xae), TORGB(0xc5), TORGB(0x8d)));//#aec58d
	an.registerFunction(quicksort_rewrap, "Qsort", glm::vec3(TORGB(0x90), TORGB(0x46), TORGB(0x71)));
	an.registerFunction(shellsort, "Shell", glm::vec3(TORGB(0x00), TORGB(0x71), TORGB(0xcc)));//#007acc
	an.registerFunction(radixsort_lsd, "Radix", glm::vec3(TORGB(0xa0), TORGB(0x60), TORGB(0xbd)));

	an.registerDataDealer(writeData);
	an.registerData(genArr);
	printf("##################################################################################\n"
		   "###                                    Analyzer                                ###\n"
		   "##################################################################################\n\n");
	printf("| Input 'exit' to stop\n");
	printf("| You can close the graph to re-enter numbers\n");
	printf("| To disable file operation, input 'io off'\n");

	//PROCESS_MEMORY_COUNTERS memCounter;
	//BOOL memResult = GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
	 // memCounter.PagefileUsage unsigned long
	//printf("PagefileUsage = %lu\n", memCounter.PagefileUsage);
	//printf("cb = %lu\n", memCounter.cb);
	//HANDLE hTimerQueue = NULL;
	//HANDLE hTimer = NULL;
	//int arg = 123;
	// Create the timer queue.
	//hTimerQueue = CreateTimerQueue();
	// Set a timer to call the timer routine in 10 seconds.
	//if (!CreateTimerQueueTimer(&hTimer, hTimerQueue,
	//	(WAITORTIMERCALLBACK)TimerRoutine, &arg, 1000, 2000, 0))
	//{
	//	printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
	//	return 3;
	//}
	while (1) {
		if (output[OUTPUT_INSTRUCTION] == 0) {
			printf("\nInput the amount of data you want to test\n");
			printf("starting ending step (seperated by spaces)\n");
		}
		fgets(buf, 1000, stdin);
		start = 0;
		end = 10000;
		step = 100;
		if (strcmp(buf, "exit\n") == 0) {
			break;
		}
		if (cmdParse(buf, an) == 0) {
			output[OUTPUT_INSTRUCTION] = 1;
		}
		else {
			if (buf[0] == '\n' || buf[0] == '\r') {
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
			//an.printMem();
			output[OUTPUT_INSTRUCTION] = 0;
			
		}
	}
	return 0;
}
