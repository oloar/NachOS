#include "usersynch.h"
#include "synch.h"
#include <map>

/*
 * Search through system's semaphore list
 * @param id : semaphore's id
 * @result : Pointer on the Sempahore if it existe in the map
 * Null othrewise
 */
Semaphore *getSemaphoreFromMap(int id) {
	Semaphore *res;
	res = semaphoreMap->find(id)->second;
	if (res == semaphoreMap->end()->second)
		res = NULL;
	return res;
}


void do_UserSemCreate() {
	DEBUG('s', "Semaphore created by user.\n");
	int id,		// Sem unique id used by user
		value;	// Sem initiale value
	id = currentSemId++; // Never set back to 0 to guaranty uniqueness
	value = machine->ReadRegister(4);// Read initiale value.
	Semaphore *sem = new Semaphore("User sempahore", value);
	semaphoreMap->insert(std::pair<int,Semaphore *>(id,sem));
	machine->WriteRegister(2, id);
}

void do_UserSemDestroy() {
	DEBUG('s', "Semaphore destroyed by user.\n");
	int id = machine->ReadRegister(4);
	Semaphore *sem = getSemaphoreFromMap(id);

	if (sem != NULL) {                      // if sem in map
		delete sem;                         // Delete it
		semaphoreMap->erase(id);             // Remove the entry from the map
	}
}

void do_UserSemP() {
	DEBUG('s', "Semaphore->P() by user.\n");
	int id = machine->ReadRegister(4);
	Semaphore *sem = getSemaphoreFromMap(id);
	if (sem == NULL)
		machine->WriteRegister(2, -1);
	else
		sem->P();
}


void do_UserSemV() {
	DEBUG('s', "Semaphore->V() by user.\n");
	int id = machine->ReadRegister(4);
	Semaphore *sem = getSemaphoreFromMap(id);
	if (sem == NULL)
		machine->WriteRegister(2, -1);
	else
		sem->V();
}
