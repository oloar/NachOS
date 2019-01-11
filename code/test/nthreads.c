#include "syscall.h"

#define N 12

void func(void * arg) {
	char s[2];
	s[0] = *(char *)arg;
	s[1] = '\n';
	s[2] = '\0';
	PutString(s);
}

int main() {

	int i;
	char s[N];
	int tids[N];
	for (i=0; i<N; i++) {
		s[i] = 'A' + i;
		tids[i] = UserThreadCreate(func, (void *)&s[i]);
		if (tids[i] == -1) {
			PutString("CREATION IMPOSSIBLE\n");
			break;
		}
	}
	return 0;
}
