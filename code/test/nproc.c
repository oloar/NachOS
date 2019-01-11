#include "syscall.h"

int main() {
	int i;
	for (i = 0; i<12; i++) {
		ForkExec("nthreads");
	}
	return 0;
}
