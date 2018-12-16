#include "syscall.h"

int main() {
	int n = 377843;

	PutInt(n);

	PutChar('\n');

	PutString("Veuillez taper un nombre :\n");

	n = GetInt();

	PutString("Vous avez tapé : ");

	PutInt(n);

	PutChar('\n');
	return 0;
}
