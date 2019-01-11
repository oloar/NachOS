#include "userconsole.h"
#include "system.h"

/*
 * read a char from reg4 and put it on the console.
 */
void do_PutChar() {
	char ch = (char) machine->ReadRegister(4);
	synchconsole->SynchPutChar(ch);
}

/*
 * read a string's address from reg4 and put it on the console.
 */
void do_PutString() {
	int addr = machine->ReadRegister(4);
	char buf[MAX_STRING_SIZE];
	machine->CopyStringFromMachine(addr, buf, MAX_STRING_SIZE);
	synchconsole->SynchPutString(buf);
}

/*
 * read a char from the console
 */
void do_GetChar() {
	char c = synchconsole->SynchGetChar();
	machine->WriteRegister(2, c);
}

/*
 * read a string from the console and copy it to reg4's address.
 */
void do_GetString() {
	int addr = machine->ReadRegister(4);
	int n = machine->ReadRegister(5);
	char buf[n];
	char * ptr = buf;
	synchconsole->SynchGetString(buf, n);
	while (machine->WriteMem(addr++, 1, *ptr) && *ptr++ != '\0');
}

/*
 * read an int from reg4 and put it to the console.
 */
void do_PutInt() {
	int n = machine->ReadRegister(4);
	synchconsole->SynchPutInt(n);
}

/*
 * read an int from the console
 */
void do_GetInt() {
	int n = synchconsole->SynchGetInt();
	machine->WriteRegister(2, n);
}
