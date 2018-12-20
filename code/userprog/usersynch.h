#ifndef USERSYNCH_H
#define USERSYNCH_H
#include "system.h"

/*
 * Create a mutex
 * write its id in reg2
 */
void do_UserMutexCreate();

/*
 * Destroy a mutex
 * reg4 should contain the mutex's id
 */
void do_UserMutexDestroy();

/*
 * Try to acquire a mutex
 * reg4 should contain the mutex's id
 */
void do_UserMutexLock();

/*
 * Unlock a mutex
 * reg4 should contains the mutex's id
 * The mutex must already be acquired by the calling thread
 */
void do_UserMutexUnlock();

/*
 * Create and initialize a semaphore
 * reg4 should contain the semaphore's initial value
 * write the semaphore's id to reg2
 */
void do_UserSemCreate();

/*
 * Destroy a semaphore
 * reg4 should contain the semaphore's id
 */
void do_UserSemDestroy();

/*
 * Perform a wait on a semaphore
 * reg4 should contain semaphore's id
 * write return code in reg2
 */
void do_UserSemP();

/*
 * Perfrom a post on a semaphore
 * reg4 should contain semaphore's id
 * write return code in reg2
 */
void do_UserSemV();

/*
 * Create a condition
 * write its id into reg2
 */
void do_UserConditionCreate();

/*
 * Destroy a condition
 * reg4 should contain the contidion's id
 */
void do_UserConditionDestroy();

/*
 * Wait on a condition
 * reg4 should contain condition's id
 * reg5 should contain mutex's id
 * writes return code to reg2
 */
void do_UserConditionWait();

/*
 * Signal on a condition
 * reg4 should contain the condition's id
 * write return code to reg2
 */
void do_UserConditionSignal();

/*
 * Broadcast on a condition
 * reg4 should contain the condition's id
 * write return code to reg2
 */
void do_UserConditionBroadcast();

#endif /* USERSYNCH_H */
