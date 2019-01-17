#include "syscall.h"

int main() {
	int i;
	int fds[15],
		created[15];
	char names[15][2] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o"};

	PutString("\n\nFile Descriptor Test ==================================\n");

	UserMkdir("test");
	UserChdir("test");
	UserMkdir("dir");
	for (i = 0; i < 15; i++) {
		if(i == 7)
			UserChdir("dir");
		created[i] = UserCreate(names[i], i);
	}
	UserChdir("..");
	
	for (i = 0; i < 15; i++) {
		if(i == 7)
			UserChdir("dir");
		PutString("Trying to open file ");
		PutString(names[i]);
		PutString(" , it should ");
		if (i < 10)
			PutString("succeed");
		else
			PutString("fail");
		fds[i] = UserOpen(names[i]);
		PutString(". Results : ");
		if (fds[i] == -1)
			PutString(" failed.\n");
		else {
			PutString(" succeeded with fd ");
			PutInt(fds[i]);
			PutChar('\n');
		}
	}

	UserChdir("..");

	for (i = 5; i < 10; i++) {
		if(i == 7)
			UserChdir("dir");
		PutString("Closing file ");
		PutString(names[i]);
		PutChar('\n');
		UserClose(fds[i]);
	}

	for (i = 10; i < 15; i++) {
		PutString("Trying to open file ");
		PutString(names[i]);
		fds[i] = UserOpen(names[i]);
		if (fds[i] == -1)
			PutString(" it failed.\n");
		else {
			PutString(" succeeded with fd ");
			PutInt(fds[i]);
			PutChar('\n');
		}
	}

	UserChdir("..");

	for (i = 0; i < 10; i++) {
		if(i == 7)
			UserChdir("dir");
		PutString("Closing file ");
		PutString(names[i]);
		PutChar('\n');
		UserClose(fds[i]);
	}

	UserChdir("..");

	for (i = 0; i < 15; i++) {
		if(i == 7)
			UserChdir("dir");
		UserRemove(names[i]);
	}
	UserChdir("..");
	UserRmdir("dir");
	UserChdir("..");
	UserRmdir("test");
	return 0;
}
