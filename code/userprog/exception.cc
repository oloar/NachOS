// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "syscall.h"
#include "system.h"
#include "userconsole.h"
#include "userthread.h"
#include "usersynch.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void UpdatePC() {
	int pc = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, pc);
	pc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, pc);
	pc += 4;
	machine->WriteRegister(NextPCReg, pc);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2);

	if (which == SyscallException) {
		switch (type) {
		case SC_Halt:
			DEBUG('e', "Shutdown, initiated by user program.\n");
			if (currentThread->tid == 0 /* est le main thread */)
				do_MainThreadExit();
			interrupt->Halt();
			break;
		case SC_Exit:
			DEBUG('e', "Exit, initiated by user program.\n");
			if (currentThread->tid == 0 /* est le main thread */)
				do_MainThreadExit();
			currentThread->Finish();
		case SC_PutChar:
			DEBUG('e', "PutChar, initiated by user program\n");
			do_PutChar();
			break;
		case SC_PutString:
			DEBUG('e', "PutString, initiated by user program\n");
			do_PutString();
			break;
		case SC_GetChar:
			DEBUG('e', "GetChar, initiated by user program\n");
			do_GetChar();
			break;
		case SC_GetString:
			DEBUG('e', "GetString, initiated by user program\n");
			do_GetString();
			break;
		case SC_PutInt:
			DEBUG('e', "PutInt, initiated by user program\n");
			do_PutInt();
			break;
		case SC_GetInt:
			DEBUG('e', "GetInt, initiated by user program\n");
			do_GetInt();
			break;
		case SC_UserThreadCreate:
			DEBUG('e', "UserThreadCreate, initiated by user program\n");
			machine->WriteRegister(2, do_UserThreadCreate(machine->ReadRegister(4), machine->ReadRegister(5)));
			break;
		case SC_UserThreadExit:
			DEBUG('e', "UserThreadExit, initiated by user program\n");
			if (currentThread->tid != 0)
				do_UserThreadExit();
			break;
		case SC_UserThreadJoin:
			DEBUG('e', "UserThreadJoin, initiated by user program\n");
			machine->WriteRegister(2, do_UserThreadJoin(machine->ReadRegister(4)));
			break;
		case SC_UserMutexCreate:
			DEBUG('e', "UserMutexCreate, initiated by user program\n");
			do_UserMutexCreate();
			break;
		case SC_UserMutexDestroy:
			DEBUG('e', "UserMutexDestroy, initiated by user program\n");
			do_UserMutexDestroy();
			break;
		case SC_UserMutexLock:
			DEBUG('e', "UserMutexLock, initiated by user program\n");
			do_UserMutexLock();
			break;
		case SC_UserMutexUnlock:
			DEBUG('e', "UserMutexUnlock, initiated by user program\n");
			do_UserMutexUnlock();
			break;
		case SC_UserSemCreate:
			DEBUG('e', "UserSemCreate, initiated by user program\n");
			do_UserSemCreate();
			break;
		case SC_UserSemDestroy:
			DEBUG('e', "UserSemDestroy, initiated by user program\n");
			do_UserSemDestroy();
			break;
		case SC_UserSemP:
			DEBUG('e', "UserSemP, initiated by user program\n");
			do_UserSemP();
			break;
		case SC_UserSemV:
			DEBUG('e', "UserSemV, initiated by user program\n");
			do_UserSemV();
			break;
		case SC_UserConditionCreate:
			DEBUG('e', "UserConditionCreate, initiated by user program\n");
			do_UserConditionCreate();
			break;
		case SC_UserConditionDestroy:
			DEBUG('e', "UserConditionDestroy, initiated by user program\n");
			do_UserConditionDestroy();
			break;
		case SC_UserConditionWait:
			DEBUG('e', "UserConditionWait, initiated by user program\n");
			do_UserConditionWait();
			break;
		case SC_UserConditionSignal:
			DEBUG('e', "UserConditionSignal, initiated by user program\n");
			do_UserConditionSignal();
			break;
		case SC_UserConditionBroadcast:
			DEBUG('e', "UserConditionBroadcast, initiated by user program\n");
			do_UserConditionBroadcast();
		case SC_ForkExec:
			DEBUG('e', "ForkExec, initiated by user program\n");
			machine->WriteRegister(2, do_ForkExec(machine->ReadRegister(4)));
			break;
		case SC_ForkWait:
			DEBUG('e', "ForkWait, initiated by user program\n");
			machine->WriteRegister(2, do_ForkWait(machine->ReadRegister(4)));
			break;
		case SC_GetPID:
			DEBUG('e', "GetPID, initiated by user program\n");
			machine->WriteRegister(2, do_GetPID());
			break;
		case SC_GetPPID:
			DEBUG('e', "GetPPID, initiated by user program\n");
			machine->WriteRegister(2, do_GetPPID());
			break;
		default:
			printf("Unexpected user MODE exception %d %d\n", which, type);
			ASSERT(FALSE);
		}
	} else {
		printf("Unexpected user exception TYPE %d %d\n", which, type);
		ASSERT(FALSE);
	}

	// LB: Do not forget to increment the pc before returning!
	UpdatePC();
	// End of addition
}
