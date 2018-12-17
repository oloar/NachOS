#include "syscall.h"

void func(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
	UserThreadExit();
}

void func2(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
	UserThreadExit();
}

int main() {
	PutString("7");
	PutString("6");
	char arg = 'A';
	char arg2 = 'B';
	/*int tid = */UserThreadCreate(func, (void *)&arg);
	/*int tid2 = */UserThreadCreate(func2, (void *)&arg2);
	// UserThreadJoin(tid);
	// UserThreadJoin(tid2);
	PutString("5");
	PutString("4");
	PutString("3");
	PutString("2");
	PutString("1");
	PutString("0");
	PutString("BOOM");
	Halt();
	return 0;
}
