#include "syscall.h"

int main() {
	PutInt(GetPID());
	PutString(" fils de ");
	PutInt(GetPPID());
	PutChar('\n');
	ForkWait(ForkExec("getpid"));
	return 0;
}
