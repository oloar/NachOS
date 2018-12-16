#include "userconsole.h"
#include "system.h"

void do_PutChar() {
	char ch = (char) machine->ReadRegister(4);
	synchconsole->SynchPutChar(ch);
}

void do_PutString() {
	int addr = machine->ReadRegister(4);
	char *buf = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);
	machine->CopyStringFromMachine(addr, buf, MAX_STRING_SIZE);
	synchconsole->SynchPutString(buf);
	free(buf);
}

void do_GetChar() {
	char c = synchconsole->SynchGetChar();
	machine->WriteRegister(2, c);
}

void do_PutInt() {
	int n = machine->ReadRegister(4);
	synchconsole->SynchPutInt(n);
}

void do_GetInt() {
	int n = synchconsole->SynchGetInt();
	machine->WriteRegister(2, n);
}

