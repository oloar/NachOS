#include "syscall.h"

void func(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
	UserThreadExit();
}


int main()
{

	PutString("Start");
	char c ='A'; 
	int tids[10];
	int i = 0;
	do {
		tids[i] =UserThreadCreate(func, (void *)&c);
		c++;
		
	} while (tids[i++] != -1);
	

	PutString("End");

	return 0;
}