#include "usersynch.h"
#include "synch.h"
#include <map>


///////////////////////////////////////////////////////////
// Mutex
///////////////////////////////////////////////////////////
/*
 * Search through system's mutex list
 * @param id : mutex's id
 * @result : Pointer on the Lock if it existe in the map
 * Null othrewise
 */
Lock *getMutexFromMap(int id) {
	Lock *res;
	res = lockMap->find(id)->second;
	if (res == lockMap->end()->second)
		res = NULL;
	return res;
}

void do_UserMutexCreate() {
	DEBUG('s', "Mutex created by user.\n");
	int id;
	id = currentMutexId++;
	Lock *lock = new Lock("User lock");
	lockMap->insert(std::pair<int,Lock *>(id,lock));
	machine->WriteRegister(2, id);
}

void do_UserMutexDestroy() {
	DEBUG('s', "Mutex destroyed by user.\n");
	int id = machine->ReadRegister(4);
	Lock *lock = getMutexFromMap(id);

	if (lock != NULL) {                      // if lock in map
		delete lock;                         // Delete it
		lockMap->erase(id);             // Remove the entry from the map
	}
}

void do_UserMutexLock() {
	DEBUG('s', "Mutex locked by user.\n");
	int id = machine->ReadRegister(4);
	Lock *lock = getMutexFromMap(id);
	if (lock == NULL)
		machine->WriteRegister(2, -1);
	else
		lock->Acquire();
}

void do_UserMutexUnlock() {
	DEBUG('s', "Mutex unlocked by user.\n");
	int id = machine->ReadRegister(4);
	Lock *lock = getMutexFromMap(id);
	if (lock == NULL)
		machine->WriteRegister(2, -1);
	else
		lock->Release();
}

///////////////////////////////////////////////////////////
// Semaphore
///////////////////////////////////////////////////////////

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
