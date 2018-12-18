#include "syscall.h"

int sem;
void sleep() {
	int i;
	int limit = 104857;
	for (i = 0; i < limit; i++);
}


void func(void * arg) {
	UserSemP(sem);
	PutChar(*(char *) arg);
	PutString(" P");
	sleep();
	PutChar(*(char *) arg);
	PutString(" V");
	UserSemV(sem);
	UserThreadExit();
}


int main(void) {
	sem = UserSemCreate(2);
	int i,
		tids[4] = {0,0,0,0};
	char args[] = {'A','B','C','D'};

	for (i = 0; i < 4; i++) {
		tids[i] = UserThreadCreate(func, (void *) &args[i]);
	}

	for (i = 0; i < 4; i++)
		UserThreadJoin(tids[i]);
	UserSemDestroy(sem);
	return 0;
}
