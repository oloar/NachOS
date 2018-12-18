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
		case SC_Halt: {
			DEBUG('a', "Shutdown, initiated by user program.\n");
			// Si c'est le thread "princiaple" (celui qui execute le main)
			if (currentThread->id == 0)
				for (int i=1; i<MAX_NB_THREADS; i++)
					currentThread->space->tids[i]->P();
			interrupt->Halt();
			break;
		}
		case SC_Exit: {
			/*
			 * Copy the return value of the program to reg2
			 * @author : Vincent Pinet
			 */
			DEBUG('a', "Exit, initiated by user program.\n");
			machine->WriteRegister(2, machine->ReadRegister(4));
			break;
		}
		case SC_PutChar: {
			do_PutChar();
			break;
		}
		case SC_PutString: {
			do_PutString();
			break;
		}
		case SC_GetChar: {
			do_GetChar();
			break;
		}
		case SC_GetString: {
			do_GetString();
			break;
		}
		case SC_PutInt: {
			do_PutInt();
			break;
		}
		case SC_GetInt: {
			do_GetInt();
			break;
		}
		case SC_UserThreadCreate:{
			DEBUG('e', "UserThreadCreate, initiated by user program\n");
			int f = machine->ReadRegister(4);
			int arg = machine->ReadRegister(5);
			machine->WriteRegister(2, do_UserThreadCreate(f, arg));
			break;}
		case SC_UserThreadExit:
			DEBUG('e', "UserThreadExit, initiated by user program\n");
			do_UserThreadExit();
			break;
		case SC_UserThreadJoin:
			DEBUG('e', "UserThreadJoin, initiated by user program\n");
			do_UserThreadJoin(machine->ReadRegister(4));
			break;
		case SC_UserSemCreate:
			do_UserSemCreate();
			break;
		case SC_UserSemDestroy:
			do_UserSemDestroy();
			break;
		case SC_UserSemP:
			do_UserSemP();
			break;
		case SC_UserSemV:
			do_UserSemV();
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
