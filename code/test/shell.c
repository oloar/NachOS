#include "syscall.h"

#define BUFFER_SIZE 128

int strcmp(char * a, char * b) {
	for (; *a!='\0' && *b!='\0' && *a==*b; a++, b++);
	return *a - *b;
}

int main() {
	char cmd[BUFFER_SIZE];
	while (1) {
		PutString("> ");
		GetString(cmd, BUFFER_SIZE);
		if (!strcmp("quit", cmd)) break;
		int pid = ForkExec(cmd);
		if (pid <= 0) PutString("\nUnknown command ");
		else ForkWait(pid);
		PutString("\n");
	}
	return 0;
}
