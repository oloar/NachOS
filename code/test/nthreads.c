#include "syscall.h"

void func(void * arg) {
	PutChar(*(char *)arg);
	PutChar('\n');
	UserThreadExit();
}

int main()
{
	PutString("Start");
	char arg1 = 'A';
	char arg2 = 'B';
	char arg3 = 'C';
	char arg4 = 'D';
	char arg5 = 'E';
	//char arg6 = 'F';
	int tid1 = UserThreadCreate(func, (void *)&arg1);
	int tid2 = UserThreadCreate(func, (void *)&arg2);
	int tid3 = UserThreadCreate(func, (void *)&arg3);
	int tid4 = UserThreadCreate(func, (void *)&arg4);
	int tid5 = UserThreadCreate(func, (void *)&arg5);
	//int tid6 = UserThreadCreate(func, (void *)&arg6);
	//int tid7 = UserThreadCreate(func, (void *)&arg1);
	UserThreadJoin(tid1);
	UserThreadJoin(tid2);
	UserThreadJoin(tid3);
	UserThreadJoin(tid4);
	UserThreadJoin(tid5);
	//UserThreadJoin(tid6);
	//UserThreadJoin(tid7);
	PutString("End");

	return 0;
}