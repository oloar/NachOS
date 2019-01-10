#include "syscall.h"

int main() {
	ForkExec("userpages0");
	PutChar('A');
	PutChar('B');
	PutChar('A');
	PutChar('A');
	PutChar('A');
	PutChar('2');
	ForkExec("userpages0");
	PutChar('A');
	PutChar('C');
	return 0;
}
