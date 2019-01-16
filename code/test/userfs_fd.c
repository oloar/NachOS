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
		PutString("Creating file ");
		PutString(names[i]);
		created[i] = UserCreate(names[i], i+1);
		if (created[i])
			PutString(" succeeded.\n");
		else
			PutString(" failed.\n");
	}
	UserChdir("..");
	
	for (i = 0; i < 15; i++) {
		if(i == 7)
			UserChdir("dir");
		PutString("Trying to open file ");
		PutString(names[i]);
		PutString(" , it should ");
		if (i < 10)
			PutString("succeed ");
		else
			PutString("fail ");
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

	PutString("Cleanup... \n");
	for (i = 0; i < 15; i++) {
		if(i == 7)
			UserChdir("dir");
		PutString("Removing file ");
		PutString(names[i]);
		PutChar('\n');
		// if (created[i])
			UserRemove(names[i]);
	}
	PutString("Content of /test/dir/ :\n");
	UserListDir();
	UserChdir("..");
	UserRmdir("dir");
	PutString("Content of /test/ :\n");
	UserListDir();
	UserChdir("..");
	UserRmdir("test");
	PutString("Content of / :\n");
	UserListDir();
	return 0;
}
