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

	// Faire avec une BitMap à terme
	machine->WriteRegister(StackReg, currentThread->space->numPages * PageSize - 16 - (currentThread->id * PageSize * 3));

	machine->Run();
}


int do_UserThreadCreate(int f, int arg)  {

	Thread * newThread = new Thread("newThread");

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
	currentThread->Finish();
}
