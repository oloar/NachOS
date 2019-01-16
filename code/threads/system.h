// system.h
//      All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "interrupt.h"
#include "scheduler.h"
#include "stats.h"
#include "thread.h"
#include "timer.h"
#include "utility.h"

// Initialization and cleanup routines
extern void Initialize(int argc, char **argv); // Initialization,
					       // called before anything else
extern void Cleanup();                         // Cleanup, called when
					       // Nachos is done.

extern Thread *currentThread;       // the thread holding the CPU
extern Thread *threadToBeDestroyed; // the thread that just finished
extern Scheduler *scheduler;        // the ready list
extern Interrupt *interrupt;        // interrupt status
extern Statistics *stats;           // performance metrics
extern Timer *timer;                // the hardware alarm clock

#ifdef USER_PROGRAM
#include <map>

#include "machine.h"
#include "synch.h"
#include "synchconsole.h"
#define MAX_STRING_SIZE 256
#include "frameprovider.h"

extern int currentMutexId;
extern std::map<int,Lock *> *lockMap;

extern int currentSemId; // used to get unique sem id, DO NOT RESET
extern std::map<int,Semaphore *> *semaphoreMap; // keeps track of every user semaphore

extern int currentConditionId;
extern std::map<int,Condition *> *conditionMap;

extern Machine *machine; // user program memory and registers
extern SynchConsole *synchconsole;
extern FrameProvider *frameProvider;
#endif

#ifdef FILESYS_NEEDED // FILESYS or FILESYS_STUB
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
#include "network_tempo.h"

//extern Network_Tempo *network_Tempo;
extern PostOffice *postOffice;
#endif

#endif // SYSTEM_H
