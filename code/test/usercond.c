#include "syscall.h"

#define MAX_RSC 10

mutex_t m;
int nb_ressource;
cond_t attente_prod;
cond_t attente_conso;

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


int main(void) {
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
	return 0;
}


