#include "synchconsole.h"
#include "copyright.h"
#include "synch.h"
#include "system.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

char *buff_string;
char *buff_string_int;

SynchConsole::SynchConsole(char *readFile, char *writeFile) {
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole() {
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch) {
	console->PutChar(ch);
	writeDone->P();
}

char SynchConsole::SynchGetChar() {
	readAvail->P();
	return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[]) {
	int i = 0;
	while (s[i] != '\0') {
		SynchPutChar(s[i]);
		i++;
	}
}

void SynchConsole::SynchGetString(char *s, int n) {
	int i = 0;
	char current = SynchGetChar();
	while (current != EOF && current != '\n' && i < n) {
		s[i] = current;
		current = SynchGetChar();
		i++;
	}

	if (current == EOF)
		s[i] = '\0';
}
