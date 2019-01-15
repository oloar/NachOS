#include "syscall.h"

int main() {
	int i;
	for (i = 0; i<45; i++)
		ForkWait(ForkExec("getpid2"));
	return 0;
}
