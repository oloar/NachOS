#include "console.h"
#include "synchconsole.h"
#include "copyright.h"
#include "system.h"
#include "synch.h"

static Semaphore * readAvail;
static Semaphore * writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char * in, char * out) {
	readAvail = new Semaphore("Semaphore Read Available", 0);
	writeDone = new Semaphore("Semaphore Write Done", 0);
	console = new Console(in, out, ReadAvail, WriteDone, 0);
	lock = new Lock("SynchConsole Lock");
}

SynchConsole::~SynchConsole() {
	delete console;
	delete lock;
	delete readAvail;
	delete writeDone;
}

char SynchConsole::SynchGetChar() {
	lock->Acquire();
	readAvail->P();
	char c = console->GetChar();
	lock->Release();
	return c;
}

void SynchConsole::SynchPutChar(char c) {
	lock->Acquire();
	console->PutChar(c);
	writeDone->P();
	lock->Release();
}

void SynchConsole::SynchPutString(char * s) {
	while (*s != '\0')
		SynchPutChar(*s++);
	SynchPutChar('\n');
}

void SynchConsole::SynchGetString(char * s, int n) {
	int i;
	for (i = 0; i<n; i++) {
		char c = SynchGetChar();
		if (c == EOF || c == '\n') break;
		s[i] = c;
	}
	s[i] = '\0';
}
