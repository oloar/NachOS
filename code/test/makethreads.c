#include "syscall.h"

void func(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
}

void func2(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
	PutString("5");
}

int main() {
	PutString("5");
	char arg = 'A';
	char arg2 = 'B';
	UserThreadCreate(func, (void *)&arg);
	UserThreadCreate(func2, (void *)&arg2);
	PutString("4");
	PutString("3");
	PutString("2");
	PutString("1");
	PutString("0");
	PutString("BOOM");
	return 0;
}
