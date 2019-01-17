#include "syscall.h"

sem_t sem;
void sleep() {
	int i;
	int limit = 104857;
	for (i = 0; i < limit; i++);
}


void func(void * arg) {
	UserSemP(sem);
	PutChar(*(char *) arg);
	PutString(" took the ressource.\n");
	sleep();
	PutChar(*(char *) arg);
	PutString(" put the ressource.\n");
	UserSemV(sem);
}


int main(void) {
	sem = UserSemCreate(3);
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
