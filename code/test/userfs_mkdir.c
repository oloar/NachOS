#include "syscall.h"

int main() {
	PutString("\n\nMkdir Test ===============================================\n");
	PutString("Creating /a/\n");
	UserMkdir("a");
	PutString("Content of / :\n");
	UserListDir();
	PutString("Creating /b/\n");
	UserMkdir("b");
	PutString("Content of / :\n");
	UserListDir();
	PutString("Moving into /a/\n");
	UserChdir("a");
	PutString("Creating /a/c/\n");
	UserMkdir("c");
	PutString("Content of /a :\n");
	UserListDir();

	PutString("Cleanup...");
	UserRmdir("c");
	UserChdir("..");
	UserRmdir("b");
	UserRmdir("a");
	PutString("	Content of / :\n");
	UserListDir();
	return 0;
}
