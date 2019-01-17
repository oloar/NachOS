#include "syscall.h"

void func(void * arg) {
	char * debut = "Debut du thread X\n";
	debut[16] = (*(char *)arg);
	PutString(debut);
	int i;
	for (i = 0; i<210000; i++);
	char * fin = "Fin du thread X\n";
	fin[14] = (*(char *)arg);
	PutString(fin);
}


int main() {
	char arg1 = '1';
	char arg2 = '2';
	UserThreadCreate(func, (void *)&arg1);
	UserThreadCreate(func, (void *)&arg2);
	PutString("Tentative de terminaison du main thread\n");
	return 0;
}
