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

void SynchConsole::SynchPutInt(const int n) {
	buff_string_int = (char *)malloc(MAX_STRING_SIZE * sizeof(char));

	snprintf(buff_string_int, MAX_STRING_SIZE, "%d", n);

	int i = 0;

	while (buff_string_int[i] != '\0') {
		SynchPutChar(buff_string_int[i]);
		i++;
	}

	free(buff_string_int);
}

int SynchConsole::SynchGetInt() {
	int n;
	int i = 0;

	buff_string_int = (char *)malloc(MAX_STRING_SIZE * sizeof(char));

	do {
		buff_string_int[i] = SynchGetChar();
		i++;
	} while (buff_string_int[i - 1] != EOF && buff_string_int[i - 1] != '\n' &&
			i < n - 1);

	if (buff_string_int[i - 1] == EOF)
		buff_string_int[i - 1] = '\0';
	else
		buff_string_int[i] = '\0';

	sscanf(buff_string_int, "%d", &n);

	free(buff_string_int);

	return n;
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
