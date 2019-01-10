#include "system.h"
#include "addrspace.h"
#include <map>

typedef struct {
	int f;
	int arg;
} thread_args;

static void StartUserThread(int data) {
	thread_args * args = (thread_args *)data;

	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();

	machine->WriteRegister(PCReg, args->f);
	machine->WriteRegister(NextPCReg, args->f + 4);
	machine->WriteRegister(RetAddrReg, currentThread->space->userexitaddr);
	machine->WriteRegister(4, args->arg);

	int stackAddr = currentThread->space->GetAddrFromId(currentThread->sectorId);
	DEBUG('t' , "New user thread %d with stack starting at %d:%d.\n", currentThread->tid, currentThread->sectorId, stackAddr);
	machine->WriteRegister(StackReg, stackAddr);

	delete (thread_args *)data;
	machine->Run();
}


int do_UserThreadCreate(int f, int arg)  {


	Thread * newThread = new Thread("newThread");
	newThread->tid = currentThread->space->currentThreadId++;

	// Abandon de la création si on n'a atteint le maximum de threads crée
	if (newThread->tid >= MAX_NB_THREADS)
		return -2;

	// On demande de la memoire pour le stack
	newThread->sectorId = currentThread->space->stackSectorMap->Find();

	// Abandon de la création si on n'a pas assez de memoire pour le stack du nouveau thread
	if (newThread->sectorId == -1)
		return -1;

	// Bidouille pour l'UserThreadExit implicit
	currentThread->space->userexitaddr = machine->ReadRegister(6);

	// On marque le thread id comme en cours d'exécution
	currentThread->space->threads[newThread->tid]->P();

	thread_args * args = new thread_args();
	args->f = f;
	args->arg = arg;

	// Lancement du thread (Fork s'occupe du partage de l'AddrSpace)
	newThread->Fork(StartUserThread, (int)args);

	return newThread->tid;
}

int do_UserThreadJoin(int tid) {
	// Si thread *tid* en cours d'exécution, on attend son UserThreadExit
	currentThread->space->threads[tid]->P();
	// On remet la ressource pour faire marcher do_MainThreadExit()
	currentThread->space->threads[tid]->V();
	return 0;
}

void do_UserThreadExit() {
	DEBUG('t', "Exiting user thread %d with sector %d.\n", currentThread->tid, currentThread->sectorId);
	// On marque la fin de l'exécution
	currentThread->space->threads[currentThread->tid]->V();
	// On rend le mémoire du stack
	currentThread->space->stackSectorMap->Clear(currentThread->sectorId);

	currentThread->Finish();
}

void do_MainThreadExit() {
	DEBUG('t', "Exiting main thread 0 with sector %d.\n", currentThread->sectorId);
	// On attend les autres threads de l'AddrSpace
	for (int i=1; i<MAX_NB_THREADS; i++)
		currentThread->space->threads[i]->P();

	// On attend les processus fils
	std::map<int, Semaphore *>::iterator it;
	for (it = currentThread->space->children->begin(); it != currentThread->space->children->end(); it++) {
		it->second->P();
	}

	// HALT
	if (currentThread->space->pid == 0)
		interrupt->Halt();

	// On signal son père que l'on a fini
	currentThread->space->root->children->find(currentThread->space->pid)->second->V();

	// Valeur du return pour le shell ?
	machine->WriteRegister(2, machine->ReadRegister(4));

	// On est le dernier thread à partager cette AddrSpace, on peut donc le supprimer
	delete currentThread->space;
}

static void StartForkExec(int data) {
	currentThread->space = (AddrSpace *)data;
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
}


int do_ForkExec(int addr) {
	char filename[MAX_STRING_SIZE];
	machine->CopyStringFromMachine(addr, filename, MAX_STRING_SIZE);

	OpenFile * executable = fileSystem->Open(filename);
	AddrSpace * newSpace;

	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		return -1;
	}
	newSpace = new AddrSpace(executable);
	newSpace->root = currentThread->space;
	currentThread->space->children->insert(std::pair<int, Semaphore *>(newSpace->pid,new Semaphore("Child", 0)));
	Thread * newFork = new Thread("newFork");
	newFork->Fork(StartForkExec, (int)newSpace);

	delete executable;
	return newSpace->pid;
}
