#include "syscall.h"

void func(void * arg) {
	int m = *(int *)arg;
	UserMutexUnlock(m);
	UserMutexUnlock(m);
	UserMutexUnlock(m);
	UserMutexLock(m);
	UserMutexLock(m);
	UserMutexLock(m);
	UserMutexLock(m);
	UserMutexLock(m);
	UserMutexLock(m);
	int i;
	for (i = 0; i<1000; i++);
	UserMutexUnlock(m);
	UserMutexUnlock(m);
	UserMutexUnlock(m);
}

int main(void) {
	int m = UserMutexCreate();
	int i;
	int tids[4];
	for (i = 0; i < 4; i++)
		tids[i] = UserThreadCreate(func, (void *)&m);

	for (i = 0; i < 4; i++)
		UserThreadJoin(tids[i]);

	UserMutexDestroy(m);
	return 0;
}
