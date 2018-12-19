#ifndef USERSYNCH_H
#define USERSYNCH_H
#include "system.h"

void do_UserMutexCreate();

void do_UserMutexDestroy();

void do_UserMutexLock();

void do_UserMutexUnlock();

void do_UserSemCreate();

void do_UserSemDestroy();

void do_UserSemP();

void do_UserSemV();

void do_UserConditionCreate();

void do_UserConditionDestroy();

void do_UserConditionWait();

void do_UserConditionSignal();

void do_UserConditionBroadcast();

#endif /* USERSYNCH_H */
