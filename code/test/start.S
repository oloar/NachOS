/* Start.s
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */

#include "syscall.h"

        .text
        .align  2

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main".
 *
 * 	NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */

	.globl __start
	.ent	__start
__start:
	jal	main
	move	$4,$0
	jal	Exit	 /* if we return from main, exit(0) */
	.end __start

/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */

	.globl Halt
	.ent	Halt
Halt:
	addiu $2,$0,SC_Halt
	syscall
	j	$31
	.end Halt

	.globl Exit
	.ent	Exit
Exit:
	addiu $2,$0,SC_Exit
	syscall
	j	$31
	.end Exit

	.globl Exec
	.ent	Exec
Exec:
	addiu $2,$0,SC_Exec
	syscall
	j	$31
	.end Exec

	.globl Join
	.ent	Join
Join:
	addiu $2,$0,SC_Join
	syscall
	j	$31
	.end Join

	.globl Create
	.ent	Create
Create:
	addiu $2,$0,SC_Create
	syscall
	j	$31
	.end Create

	.globl Open
	.ent	Open
Open:
	addiu $2,$0,SC_Open
	syscall
	j	$31
	.end Open

	.globl Read
	.ent	Read
Read:
	addiu $2,$0,SC_Read
	syscall
	j	$31
	.end Read

	.globl Write
	.ent	Write
Write:
	addiu $2,$0,SC_Write
	syscall
	j	$31
	.end Write

	.globl Close
	.ent	Close
Close:
	addiu $2,$0,SC_Close
	syscall
	j	$31
	.end Close

	.globl Fork
	.ent	Fork
Fork:
	addiu $2,$0,SC_Fork
	syscall
	j	$31
	.end Fork

	.globl Yield
	.ent	Yield
Yield:
	addiu $2,$0,SC_Yield
	syscall
	j	$31
	.end Yield

	.globl PutChar
	.ent	PutChar
PutChar:
	addiu $2,$0,SC_PutChar
	syscall
	j	$31
	.end PutChar

	.globl GetChar
	.ent	GetChar
GetChar:
	addiu $2,$0,SC_GetChar
	syscall
	j	$31
	.end GetChar

	.globl PutString
	.ent	PutString
PutString:
	addiu $2,$0,SC_PutString
	syscall
	j	$31
	.end PutString

	.globl GetString
	.ent	GetString
GetString:
	addiu $2,$0,SC_GetString
	syscall
	j	$31
	.end GetString

	.globl PutInt
	.ent	PutInt
PutInt:
	addiu $2,$0,SC_PutInt
	syscall
	j	$31
	.end PutInt

	.globl GetInt
	.ent	GetInt
GetInt:
	addiu $2,$0,SC_GetInt
	syscall
	j	$31
	.end GetInt

	.globl UserThreadCreate
	.ent	UserThreadCreate
UserThreadCreate:
	addiu $2,$0,SC_UserThreadCreate
	addiu $6,$0,UserThreadExit
	syscall
	j	$31
	.end UserThreadCreate

	.globl UserThreadExit
	.ent	UserThreadExit
UserThreadExit:
	addiu $2,$0,SC_UserThreadExit
	syscall
	j	$31
	.end UserThreadExit

	.globl UserThreadJoin
	.ent	UserThreadJoin
UserThreadJoin:
	addiu $2,$0,SC_UserThreadJoin
	syscall
	j	$31
	.end UserThreadJoin

	.globl UserMutexCreate
	.ent	UserMutexCreate
UserMutexCreate:
	addiu $2,$0,SC_UserMutexCreate
	syscall
	j	$31
	.end UserMutexCreate

	.globl UserMutexDestroy
	.ent	UserMutexDestroy
UserMutexDestroy:
	addiu $2,$0,SC_UserMutexDestroy
	syscall
	j	$31
	.end UserMutexDestroy

	.globl UserMutexLock
	.ent	UserMutexLock
UserMutexLock:
	addiu $2,$0,SC_UserMutexLock
	syscall
	j	$31
	.end UserMutexLock

	.globl UserMutexUnlock
	.ent	UserMutexUnlock
UserMutexUnlock:
	addiu $2,$0,SC_UserMutexUnlock
	syscall
	j	$31
	.end UserMutexUnlock

	.globl UserSemCreate
	.ent	UserSemCreate
UserSemCreate:
	addiu $2,$0,SC_UserSemCreate
	syscall
	j	$31
	.end UserSemCreate

	.globl UserSemDestroy
	.ent	UserSemDestroy
UserSemDestroy:
	addiu $2,$0,SC_UserSemDestroy
	syscall
	j	$31
	.end UserSemDestroy

	.globl UserSemP
	.ent	UserSemP
UserSemP:
	addiu $2,$0,SC_UserSemP
	syscall
	j	$31
	.end UserSemP

	.globl UserSemV
	.ent	UserSemV
UserSemV:
	addiu $2,$0,SC_UserSemV
	syscall
	j	$31
	.end UserSemV

	.globl UserConditionCreate
	.ent	UserConditionCreate
UserConditionCreate:
	addiu $2,$0,SC_UserConditionCreate
	syscall
	j	$31
	.end UserConditionCreate

	.globl UserConditionDestroy
	.ent	UserConditionDestroy
UserConditionDestroy:
	addiu $2,$0,SC_UserConditionDestroy
	syscall
	j	$31
	.end UserConditionDestroy

	.globl UserConditionWait
	.ent	UserConditionWait
UserConditionWait:
	addiu $2,$0,SC_UserConditionWait
	syscall
	j	$31
	.end UserConditionWait

	.globl UserConditionSignal
	.ent	UserConditionSignal
UserConditionSignal:
	addiu $2,$0,SC_UserConditionSignal
	syscall
	j	$31
	.end UserConditionSignal

	.globl UserConditionBroadcast
	.ent	UserConditionBroadcast
UserConditionBroadcast:
	addiu $2,$0,SC_UserConditionBroadcast
	syscall
	j	$31
	.end UserConditionBroadcast

	.globl ForkExec
	.ent	ForkExec
ForkExec:
	addiu $2,$0,SC_ForkExec
	syscall
	j	$31
	.end ForkExec

	.globl ForkWait
	.ent	ForkWait
ForkWait:
	addiu $2,$0,SC_ForkWait
	syscall
	j	$31
	.end ForkWait

	.globl UserMkdir
	.ent	UserMkdir
UserMkdir:
	addiu $2,$0,SC_UserMkdir
	syscall
	j	$31
	.end UserMkdir

	.globl UserRmdir
	.ent	UserRmdir
UserRmdir:
	addiu $2,$0,SC_UserRmdir
	syscall
	j	$31
	.end UserRmdir

	.globl UserChdir
	.ent	UserChdir
UserChdir:
	addiu $2,$0,SC_UserChdir
	syscall
	j	$31
	.end UserChdir

	.globl UserListDir
	.ent	UserListDir
UserListDir:
	addiu $2,$0,SC_UserListDir
	syscall
	j	$31
	.end UserListDir

	.globl UserCreate
	.ent	UserCreate
UserCreate:
	addiu $2,$0,SC_UserCreate
	syscall
	j	$31
	.end UserCreate

	.globl UserRemove
	.ent	UserRemove
UserRemove:
	addiu $2,$0,SC_UserRemove
	syscall
	j	$31
	.end UserRemove

	.globl UserOpen
	.ent	UserOpen
UserOpen:
	addiu $2,$0,SC_UserOpen
	syscall
	j	$31
	.end UserOpen

	.globl UserClose
	.ent	UserClose
UserClose:
	addiu $2,$0,SC_UserClose
	syscall
	j	$31
	.end UserClose

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main
