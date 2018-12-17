#include "system.h"

static int counter = 0;

typedef struct {
	int f;
	int arg;
	int sp;
} triplet;

static void StartUserThread(int data) {
	triplet * args = (triplet *)data;

	// Pas compris l'utilité mais marche sans. ???
	// currentThread->space->RestoreState();
	// currentThread->space->InitRegisters();

	machine->WriteRegister(PCReg, args->f);
	machine->WriteRegister(NextPCReg, args->f + 4);
	machine->WriteRegister(4, args->arg);

	// Notez que vous aurez à initialiser le pointeur de pile. Il vous est suggéré de le placer 2 ou 3 pages en
	// dessous du pointeur du programme principal. Ceci est une évaluation empirique, bien sûr ! Il faudra
	// probablement faire mieux dans un deuxième temps...
	machine->WriteRegister(StackReg, args->sp - PageSize * 3);

	machine->Run();
}


int do_UserThreadCreate(int f, int arg)  {

	// Creation d'un nouveau thread
	counter++;
	Thread * newThread = new Thread("Thread#"/*Counter*/);

	triplet * args = new triplet();
	args->f = f;
	args->arg = arg;
	// Sauvegarde de SP car vaudra 0 une fois dans StartUserThread()
	args->sp = machine->ReadRegister(StackReg);

	// Mise dans la file d'attente (Fork s'occupe de AddrSpace)
	newThread->Fork(StartUserThread, (int)args);

	return counter;
}

int do_UserThreadJoin(int tid) {
	// TODO
	return 0;
}

void do_UserThreadExit(void) {
	currentThread->Finish();
	// N’oubliez pas de détruire aussi les structures AddrSpace. ??? Pas de suite car elle est partagée ???
}
