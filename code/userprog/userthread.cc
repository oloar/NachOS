#include "system.h"
#include "addrspace.h"

typedef struct {
	int f;
	int arg;
} duo;

static void StartUserThread(int data) {
	duo * args = (duo *)data;

	// Pas compris l'utilité mais marche sans. ???
	currentThread->space->RestoreState();
	// currentThread->space->InitRegisters();

	machine->WriteRegister(PCReg, args->f);
	machine->WriteRegister(NextPCReg, args->f + 4);
	machine->WriteRegister(4, args->arg);

	// Notez que vous aurez à initialiser le pointeur de pile. Il vous est suggéré de le placer 2 ou 3 pages en
	// dessous du pointeur du programme principal. Ceci est une évaluation empirique, bien sûr ! Il faudra
	// probablement faire mieux dans un deuxième temps...
	int stackAddr = currentThread->space->GetAddrFromId(currentThread->sectorId);
	DEBUG('l' , "New user thread %d with stack starting at %d:%d.\n", currentThread->id, currentThread->sectorId, stackAddr);
	machine->WriteRegister(StackReg,stackAddr);

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
	DEBUG('l', "Exiting user threads %d with sector %d.\n", currentThread->id, currentThread->sectorId);
	currentThread->space->tids[currentThread->id]->V();
	currentThread->space->stackSectorMap->Clear(currentThread->sectorId);
	currentThread->Finish();
}

static void StartForkExec(int data) {
	currentThread->space->InitRegisters(); // set the initial register values
	currentThread->space->RestoreState();
	machine->Run();
}


int do_ForkExec(int addr) {
	printf("==%p\n", currentThread->space);
	char * filename = new char[MAX_STRING_SIZE];
	machine->CopyStringFromMachine(addr, filename, MAX_STRING_SIZE);

	OpenFile *executable = fileSystem->Open(filename);
	AddrSpace *space;

	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		return -1;
	}
	space = new AddrSpace(executable);
	printf("==%p\n", space);
	Thread * newFork = new Thread("newFork");
	newFork->Fork(StartForkExec, 0);
	newFork->space = space;

	// delete filename;
	// delete executable; // close file
	return 0;
}
