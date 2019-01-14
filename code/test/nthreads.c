#include "syscall.h"

#define N 120

void func(void * arg) {
	int me = *(int *)arg;
	char s[5];
	s[0] = me/100 + '0';
	s[1] = me/10%10 + '0';
	s[2] = me%10 + '0';
	s[3] = '\n';
	s[4] = '\0';
	PutString(s);

}

int main() {

	int i, j;
	int s[N];
	int tids[N];
	for (i=0; i<N; i++) {
		s[i] = 	i;
		tids[i] = UserThreadCreate(func, (void *)&s[i]);
		if (tids[i] == -1)  {
			PutString("Cannot Create now, waiting a bit\n");
			for(j=0;j<10000;j++);
			i--;
		}
	}

	for (i=0; i<N; i++) {
		PutString("tids[");
		PutInt(i);
		PutString("] = ");
		PutInt(tids[i]);
		PutChar('\n');
	}
	return 0;
}
