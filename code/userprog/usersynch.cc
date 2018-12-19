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
	int id,     // Sem unique id used by user
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

///////////////////////////////////////////////////////////
// Condition
///////////////////////////////////////////////////////////

/*
 * Search through system's condition list
 * @param id : condition's id
 * @result : Pointer on the condition if it existe in the map
 * Null othrewise
 */
Condition *getConditionFromMap(int id) {
	Condition *res;
	res = conditionMap->find(id)->second;
	if (res == conditionMap->end()->second)
		res = NULL;
	return res;
}


void do_UserConditionCreate() {
	DEBUG('s', "Condition created by user.\n");
	int id;                                                     // unique id used by user
	id = currentConditionId++;                                  // Never set back to 0 to guaranty uniqueness
	Condition *cond = new Condition("User condition");
	conditionMap->insert(std::pair<int,Condition *>(id,cond));
	machine->WriteRegister(2, id);
}

void do_UserConditionDestroy() {
	DEBUG('s', "Condition destroyed by user.\n");
	int id = machine->ReadRegister(4);
	Condition *cond = getConditionFromMap(id);

	if (cond != NULL) {                                          // if cond in map
		delete cond;                                             // Delete it
		conditionMap->erase(id);                                // Remove the entry from the map
	}
}

void do_UserConditionWait() {
	DEBUG('s', "Condition wait by user.\n");
	int condId = machine->ReadRegister(4);
	int mutexId = machine->ReadRegister(5);
	Condition *cond = getConditionFromMap(condId);
	Lock *mutex = getMutexFromMap(mutexId);
	if (cond == NULL || mutex == NULL)
		machine->WriteRegister(2, -1);
	else
		cond->Wait(mutex);
}

void do_UserConditionSignal() {
	DEBUG('s', "Condition signal by user.\n");
	int condId = machine->ReadRegister(4);
	Condition *cond = getConditionFromMap(condId);
	if (cond == NULL)
		machine->WriteRegister(2, -1);
	else
		cond->Signal();
}

void do_UserConditionBroadcast() {
	DEBUG('s', "Condition signal by user.\n");
	int condId = machine->ReadRegister(4);
	Condition *cond = getConditionFromMap(condId);
	if (cond == NULL)
		machine->WriteRegister(2, -1);
	else
		cond->Broadcast();
}
