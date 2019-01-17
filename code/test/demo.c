#include "syscall.h"

#define RUNFOLDER "run.d"

void setup_folders() {
	UserMkdir(RUNFOLDER);
}

int main() {
	int pid;
	setup_folders();
	PutString("Setup complete, launching shell.\n");
	pid = ForkExec("shell");
	ForkWait(pid);
	return 0;
}
