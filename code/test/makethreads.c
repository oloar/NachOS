#include "syscall.h"

void func(void * arg) {
	PutChar('B');
	PutChar('\n');
	PutChar('C');
	PutChar('\n');
	UserThreadExit();
}

int main() {
	PutChar('A');
	PutChar('\n');
	int arg = 42;
	UserThreadCreate(func, (void *)&arg);
	PutChar('D');
	PutChar('\n');
	Halt();
	return 0;
}
