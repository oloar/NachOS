#include "usersynch.h"
#include "synch.h"
#include <map>

#define USERSYNCH_ERROR 1

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
	int id;
	id = currentMutexId++;
	Lock *lock = new Lock("User lock");
	lockMap->insert(std::pair<int,Lock *>(id,lock));
	DEBUG('s', "Mutex created by user's thread %d.\n", currentThread->id);
	machine->WriteRegister(2, id);
}

void do_UserMutexDestroy() {
	int id = machine->ReadRegister(4);
	Lock *lock = getMutexFromMap(id);

	if (lock != NULL) {                      // if lock in map
		DEBUG('s', "Mutex destroyed by user's thread %d.\n", currentThread->id);
		delete lock;                         // Delete it
		lockMap->erase(id);             // Remove the entry from the map
	}
}

void do_UserMutexLock() {
	int id = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;

	Lock *lock = getMutexFromMap(id);
	if (lock != NULL && !lock->isHeldByCurrentThread()) {
		lock->Acquire();
		DEBUG('s', "Mutex locked by user's thread %d.\n", currentThread->id);
		res = 0;
	}
	machine->WriteRegister(2, res);
}

void do_UserMutexUnlock() {
	int id = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;

	Lock *lock = getMutexFromMap(id);
	if (lock != NULL && lock->isHeldByCurrentThread()) {
		DEBUG('s', "Mutex unlocked by user's thread %d.\n", currentThread->id);
		lock->Release();
		res = 0;
	}
	machine->WriteRegister(2, res);
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
	DEBUG('s', "Semaphore created by user's thread %d.\n", currentThread->id);
	int id,     // Sem unique id used by user
		value;	// Sem initiale value
	id = currentSemId++; // Never set back to 0 to guaranty uniqueness
	value = machine->ReadRegister(4);// Read initiale value.
	Semaphore *sem = new Semaphore("User sempahore", value);
	semaphoreMap->insert(std::pair<int,Semaphore *>(id,sem));
	machine->WriteRegister(2, id);
}

void do_UserSemDestroy() {
	DEBUG('s', "Semaphore destroyed by user's thread %d.\n", currentThread->id);
	int id = machine->ReadRegister(4);
	Semaphore *sem = getSemaphoreFromMap(id);

	if (sem != NULL) {                      // if sem in map
		delete sem;                         // Delete it
		semaphoreMap->erase(id);             // Remove the entry from the map
	}
}

void do_UserSemP() {
	int id = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;

	Semaphore *sem = getSemaphoreFromMap(id);
	if (sem != NULL) {
		sem->P();
		res = 0;
		DEBUG('s', "Semaphore->P() by user's thread %d.\n", currentThread->id);
	}
	machine->WriteRegister(2, res);
}


void do_UserSemV() {
	int id = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;

	Semaphore *sem = getSemaphoreFromMap(id);
	if (sem != NULL) {
		sem->V();
		res = 0;
		DEBUG('s', "Semaphore->V() by user's thread %d.\n", currentThread->id);
	}
	machine->WriteRegister(2, res);
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
	DEBUG('s', "Condition created by user's thread %d.\n", currentThread->id);
	int id;                                                     // unique id used by user
	id = currentConditionId++;                                  // Never set back to 0 to guaranty uniqueness
	Condition *cond = new Condition("User condition");
	conditionMap->insert(std::pair<int,Condition *>(id,cond));
	machine->WriteRegister(2, id);
}

void do_UserConditionDestroy() {
	int id = machine->ReadRegister(4);
	Condition *cond = getConditionFromMap(id);

	if (cond != NULL) {                                          // if cond in map
		DEBUG('s', "Condition destroyed by user's thread %d.\n", currentThread->id);
		delete cond;                                             // Delete it
		conditionMap->erase(id);                                // Remove the entry from the map
	}
}

void do_UserConditionWait() {
	int condId = machine->ReadRegister(4);
	int mutexId = machine->ReadRegister(5);
	int res = USERSYNCH_ERROR;
	Condition *cond = getConditionFromMap(condId);
	Lock *mutex = getMutexFromMap(mutexId);

	if (cond != NULL && mutex != NULL) {
		DEBUG('s', "Condition wait by user's thread %d.\n", currentThread->id);
		cond->Wait(mutex);
		res = 0;
	}
	machine->WriteRegister(2, res);
}

void do_UserConditionSignal() {
	int condId = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;
	Condition *cond = getConditionFromMap(condId);
	if (cond != NULL) {
		DEBUG('s', "Condition signal by user's thread %d.\n", currentThread->id);
		cond->Signal();
		res = 0;
	}
	machine->WriteRegister(2, res);
}

void do_UserConditionBroadcast() {
	int condId = machine->ReadRegister(4);
	int res = USERSYNCH_ERROR;
	Condition *cond = getConditionFromMap(condId);
	if (cond != NULL) {
		DEBUG('s', "Condition signal by user's thread %d.\n", currentThread->id);
		cond->Broadcast();
		res = 0;
	}
	machine->WriteRegister(2, res);
}
