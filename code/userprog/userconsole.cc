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
	char *buf = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);
	machine->CopyStringFromMachine(addr, buf, MAX_STRING_SIZE);
	synchconsole->SynchPutString(buf);
	free(buf);
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
	char *buf;
	char *addr = (char *) machine->ReadRegister(4);
	int n = (int) machine->ReadRegister(5);
	buf = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);
	synchconsole->SynchGetString(buf, n); // TODO : SynchGetString to return size
	for (int i = 0; i < n; i++) {
		if (buf[i] == '\0')
			break;
		machine->WriteMem((int)addr + i, 1, (int) buf[i]);
	}
	free(buf);
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

