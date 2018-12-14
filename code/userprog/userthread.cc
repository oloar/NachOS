#include "system.h"

static void StartUserThread(int f) {
	// /!\ ARGS

	printf("-StartUserThread()\n");
	// Mise a jour ??
	// currentThread->space->InitRegisters();
	// currentThread->space->RestoreState();

	machine->WriteRegister(PCReg, f);
	machine->WriteRegister(NextPCReg, f + 4);
	
	// Notez que vous aurez à initialiser le pointeur de pile. Il vous est suggéré de le placer 2 ou 3 pages en
	// dessous du pointeur du programme principal. Ceci est une évaluation empirique, bien sûr ! Il faudra
	// probablement faire mieux dans un deuxième temps...
	machine->WriteRegister(StackReg, 2000 + 3 * PageSize);

	// machine->Run(); // On doit lancer la machine qu'une seule fois ?
}


int do_UserThreadCreate(int f, int arg)  {
	printf("-do_UserThreadCreate()\n");

	// Creation d'un nouveau thread
	Thread * newThread = new Thread("New UserThreadCreated");

	// Mise dans la file d'attente
	// S'occupe dua AddrSpace
	newThread->Fork(StartUserThread, f);

	// Attente artificiel ?
	currentThread->Yield();
	return 0;
}

void do_UserThreadExit(void) {
	currentThread->Finish();
	// N’oubliez pas de détruire aussi les structures AddrSpace. ?? Non car il est partagé ??
}
