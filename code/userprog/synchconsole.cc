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
	console = new Console(in, out, ReadAvail, WriteDone, 0);
	readAvail = new Semaphore("Semaphore Read Available", 0);
	writeDone = new Semaphore("Semaphore Write Done", 0);
	charlock = new Lock("SynchConsole CharLock");
	stringlock = new Lock("SynchConsole StringLock");
}

SynchConsole::~SynchConsole() {
	delete console;
	delete readAvail;
	delete writeDone;
	delete charlock;
	delete stringlock;
}

char SynchConsole::SynchGetChar() {
	charlock->Acquire();
	readAvail->P();
	char c = console->GetChar();
	charlock->Release();
	return c;
}

void SynchConsole::SynchPutChar(char c) {
	charlock->Acquire();
	console->PutChar(c);
	writeDone->P();
	charlock->Release();
}

void SynchConsole::SynchPutString(char * s) {
	stringlock->Acquire();
	while (*s != '\0')
		SynchPutChar(*s++);
	stringlock->Release();
}

void SynchConsole::SynchGetString(char * s, int n) {
	stringlock->Acquire();
	int i;
	for (i = 0; i < n - 1; i++) {
		char c = SynchGetChar();
		if (c == EOF || c == '\n') break;
		s[i] = c;
	}
	s[i] = '\0';
	stringlock->Release();
}

void SynchConsole::SynchPutInt(const int n) {
	stringlock->Acquire();
	char buff[MAX_STRING_SIZE];
	snprintf(buff, MAX_STRING_SIZE, "%d", n);
	for (int i = 0; buff[i] != '\0'; i++)
		SynchPutChar(buff[i]);
	stringlock->Release();
}

int SynchConsole::SynchGetInt() {
	stringlock->Acquire();
	// Copie du code GetString pour eviter un autre verrou
	char buffer[MAX_STRING_SIZE];
	int i;
	for (i = 0; i<MAX_STRING_SIZE - 1; i++) {
		char c = SynchGetChar();
		if (c == EOF || c == '\n') break;
		buffer[i] = c;
	}
	buffer[i] = '\0';

	int res = 0;
	sscanf(buffer, "%d", &res);
	stringlock->Release();
	return res;
}
