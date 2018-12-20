#include "syscall.h"

int m;
void sleep() {
	int i;
	int limit = 100000;
	for (i = 0; i < limit; i++);
}



void func(void * arg) {
	UserMutexLock(m);
	sleep();
	UserMutexUnlock(m);
}

int main(void) {
	m = UserMutexCreate();
	int i,
		tids[4] = {0,0,0,0};
	char args[] = {'A','B','C','D'};

	for (i = 0; i < 4; i++) {
		tids[i] = UserThreadCreate(func, (void *) &args[i]);
	}

	for (i = 0; i < 4; i++)
		UserThreadJoin(tids[i]);
	UserMutexDestroy(m);
	return 0;
}

