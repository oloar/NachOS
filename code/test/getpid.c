#include "syscall.h"

int main() {
	PutInt(GetPID());
	PutString(" fils de ");
	PutInt(GetPPID());
	PutChar('\n');
	return 0;
}
