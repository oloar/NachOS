#include "system.h"
#include "addrspace.h"

typedef struct {
	int f;
	int arg;
} duo;

static void StartUserThread(int data) {
	duo * args = (duo *)data;

	// Pas compris l'utilité mais marche sans. ???
	// currentThread->space->RestoreState();
	// currentThread->space->InitRegisters();

	machine->WriteRegister(PCReg, args->f);
	machine->WriteRegister(NextPCReg, args->f + 4);
	machine->WriteRegister(4, args->arg);

	// Notez que vous aurez à initialiser le pointeur de pile. Il vous est suggéré de le placer 2 ou 3 pages en
	// dessous du pointeur du programme principal. Ceci est une évaluation empirique, bien sûr ! Il faudra
	// probablement faire mieux dans un deuxième temps...
	int temp = currentThread->space->gsize - 16 - (currentThread->sectorId * ThreadStackSize);;
	DEBUG('l' , "valeur stack %d ",temp);
	machine->WriteRegister(StackReg,temp);

	machine->Run();
}


int do_UserThreadCreate(int f, int arg)  {

	Thread * newThread = new Thread("newThread");

	newThread->sectorId = currentThread->space->stackSectorMap->Find();
	if (newThread->sectorId == -1) return -1;

	duo * args = new duo();
	args->f = f;
	args->arg = arg;

	// Mise dans la file d'attente (Fork s'occupe de AddrSpace)
	newThread->Fork(StartUserThread, (int)args);
	newThread->space->tids[newThread->id]->P();
	return newThread->id;
}

int do_UserThreadJoin(int tid) {
	// On attends une ressource (aka UserThreadExit)
	currentThread->space->tids[tid]->P();
	// On remet la ressource pour symbolisé que le tid est disponible
	currentThread->space->tids[tid]->V();
	return 0;
}

void do_UserThreadExit(void) {
	currentThread->space->tids[currentThread->id]->V();
	currentThread->space->stackSectorMap->Clear(currentThread->sectorId);
	currentThread->Finish();
}
