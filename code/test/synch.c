#include "syscall.h"

#define MAX_RSC 10

mutex_t m;
sem_t sem;
int nb_ressource;
cond_t attente_prod;
cond_t attente_conso;

void sleep() {
	int i;
	int limit = 100000;
	for (i = 0; i < limit; i++);
}

/*
 * Mutex demonstration
 */
void func_mutex(void * arg) {
	UserMutexLock(m);
	PutString("Thread ");
	PutChar(*(char *) arg);
	PutString(" owns the lock.\n");
	sleep();
	PutString("Thread ");
	PutChar(*(char *) arg);
	PutString(" releases the lock.\n");
	UserMutexUnlock(m);
}

void mutex_test() {
	PutString("Running mutex demonstration.\n");
	m = UserMutexCreate();
	int i,
		tids[4] = {0,0,0,0};
	char args[] = {'A','B','C','D'};

	for (i = 0; i < 4; i++) {
		tids[i] = UserThreadCreate(func_mutex, (void *) &args[i]);
	}

	for (i = 0; i < 4; i++)
		UserThreadJoin(tids[i]);
	UserMutexDestroy(m);
}

/*
 * Semaphore demonstration
 */
void func_sem(void * arg) {
	UserSemP(sem);
	sleep();
	UserSemV(sem);
	UserThreadExit();
	UserSemP(m);
	PutString("Thread ");
	PutChar(*(char *) arg);
	PutString(" took the ressource.\n");
	sleep();
	PutString("Thread ");
	PutChar(*(char *) arg);
	PutString(" put the ressource.\n");
	UserSemV(m);
}


void sem_test() {
	sem = UserSemCreate(2);
	int i,
		tids[4] = {0,0,0,0};
	char args[] = {'A','B','C','D'};

	for (i = 0; i < 4; i++) {
		tids[i] = UserThreadCreate(func_sem, (void *) &args[i]);
	}

	for (i = 0; i < 4; i++)
		UserThreadJoin(tids[i]);
	UserSemDestroy(sem);
}

/*
 * Condition demonstration
 */
void prod(void * arg) {
	int i;
	for (i = 0; i < 5; i++) {
		UserMutexLock(m);
		while(nb_ressource == MAX_RSC)
			UserConditionWait(attente_prod, m);
		nb_ressource++;

		PutString("Prod ");
		PutChar(*(char *) arg);
		PutInt(nb_ressource);
		PutChar('\n');

		UserConditionSignal(attente_conso);
		UserMutexUnlock(m);
	}
}

void conso(void * arg) {
	int i;
	for (i = 0; i < 5; i++) {
		UserMutexLock(m);
		while (nb_ressource == 0)
			UserConditionWait(attente_conso,m);
		nb_ressource--;

		PutString("Conso ");
		PutChar(*(char *) arg);
		PutInt(nb_ressource);
		PutChar('\n');

		UserConditionSignal(attente_prod);
		UserMutexUnlock(m);
	}
}


void cond_test() {
	m = UserMutexCreate();
	attente_prod = UserConditionCreate();
	attente_conso = UserConditionCreate();
	int i,
		tids[] = {0,0,0,0,0,0,0,0};
	char args[] = {'A','B','C','D','E','F','G','H'};

	nb_ressource = 0;

	for (i = 0; i < 4; i++)
		tids[i] = UserThreadCreate(prod, (void *) &args[i]);
	for (i = 4; i < 8; i++)
		tids[i] = UserThreadCreate(conso, (void *) &args[i]);

	for (i = 0; i < 8; i++)
		if (tids[i] != -1)
			UserThreadJoin(tids[i]);

	UserConditionDestroy(attente_conso);
	UserConditionDestroy(attente_prod);
	UserMutexDestroy(m);
}

int main() {
	int quit = 0;
	char c;
	while(!quit) {
		PutString("Tests :\n");
		PutString("1 - Mutex\n");
		PutString("2 - Semaphore\n");
		PutString("3 - Conditions\n");
		PutString("4 - Quit\n");
		PutString("Which test would you like to run?\n");
		c = GetChar();
		switch(c) {
			case '1' :
				mutex_test();
				break;
			case '2' :
				sem_test();
				break;
			case '3' :
				cond_test();
				break;
			case '4' :
				quit = 1;
				break;
			default:
				break;
		}
	}

	return 0;
}
