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

