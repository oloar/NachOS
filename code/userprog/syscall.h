/* syscalls.h
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel.
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SC_Halt 0
#define SC_Exit 1
#define SC_Exec 2
#define SC_Join 3
#define SC_Create 4
#define SC_Open 5
#define SC_Read 6
#define SC_Write 7
#define SC_Close 8
#define SC_Fork 9
#define SC_Yield 10
#define SC_PutChar 11
#define SC_GetChar 12
#define SC_PutString 13
#define SC_GetString 14
#define SC_PutInt 15
#define SC_GetInt 16
#define SC_UserThreadCreate 17
#define SC_UserThreadExit 18
#define SC_UserThreadJoin 19
#define SC_UserMutexCreate 20
#define SC_UserMutexDestroy 21
#define SC_UserMutexLock 22
#define SC_UserMutexUnlock 23
#define SC_UserSemCreate 24
#define SC_UserSemDestroy 25
#define SC_UserSemP 26
#define SC_UserSemV 27
#define SC_UserConditionCreate 28
#define SC_UserConditionDestroy 29
#define SC_UserConditionWait 30
#define SC_UserConditionSignal 31
#define SC_UserConditionBroadcast 32

#ifdef IN_USER_MODE

// LB: This part is read only on compiling the test/*.c files.
// It is *not* read on compiling test/start.S

/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking,
 * from the system call entry point in exception.cc.
 */

/* Stop Nachos, and print out performance stats */
void Halt() __attribute__((noreturn));

/* Address space control operations: Exit, Exec, and Join */

/* This user program is done (status = 0 means exited normally). */
void Exit(int status) __attribute__((noreturn));

/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;

/* Run the executable, stored in the Nachos file "name", and return the
 * address space identifier
 */
SpaceId Exec(char *name);

/* Only return once the the user program "id" has finished.
 * Return the exit status.
 */
int Join(SpaceId id);

/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */

/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;

/* when an address space starts up, it has two open files, representing
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput 0
#define ConsoleOutput 1

/* Create a Nachos file, with "name" */
void Create(char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can
 * be used to read and write to the file.
 */
OpenFileId Open(char *name);

/* Write "size" bytes from "buffer" to the open file. */
void Write(char *buffer, int size, OpenFileId id);

/* Read "size" bytes from the open file into "buffer".
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough
 * characters to read, return whatever is available (for I/O devices,
 * you should always wait until you can return at least one character).
 */
int Read(char *buffer, int size, OpenFileId id);

/* Close the file, we're done reading and writing to it. */
void Close(OpenFileId id);

/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program.
 */

/* Fork a thread to run a procedure ("func") in the *same* address space
 * as the current thread.
 */
void Fork(void (*func)());

/* Yield the CPU to another runnable thread, whether in this address space
 * or not.
 */
void Yield();

/**
 * Put a char to the console
 * @param c : the char
 */
void PutChar(char c);

/*
 * Read a char from the console
 * @result : the char
 */
char GetChar();

/*
 * Write a string to the console
 * @param s : the string to write
 */
void PutString(char * s);

/*
 * Read a string from the console
 * @param s : the string
 * @param n : max number of characters to read
 */
void GetString(char * s, int n);

/*
 * Write an int to the console
 * @param n : the int
 */
void PutInt(int n);

/*
 * Read an int from the console
 * @result : the int
 */
int GetInt();

/*
 * Create a thread running a function
 * @param f : pointer to the function to be runned
 * @param arg : pointer to the function's arguments
 */
int UserThreadCreate(void (*f)(void *), void * arg);

/*
 * Exit the current thread
 */
void UserThreadExit();

/*
 * Wait for a thread to finish
 * @param tid : the thread to wait id
 * @result : TODO
 */
int UserThreadJoin(int tid);

int UserMutexCreate();
int UserMutexDestroy(int id);
int UserMutexLock(int id);
int UserMutexUnlock(int id);

int UserSemCreate();
int UserSemDestroy(int id);
int UserSemP(int id);
int UserSemV(int id);

int UserConditionCreate();
void UserConditionDestroy(int condId);
void UserConditionWait(int condId, int mutexId);
void UserConditionSignal(int condId); //, int mutexId);
void UserConditionBroadcast(int condId); // , int mutexId);

#endif // IN_USER_MODE

#endif /* SYSCALL_H */
