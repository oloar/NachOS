#include "userfs.h"

#ifdef FILESYS_STUB

void do_UserMkdir() {
	printf("UserMkdir\n");
}

void do_UserRmdir() {
	printf("UserRmdir\n");
}

void do_UserChdir() {
	printf("UserChdir\n");
}

void do_UserListDir() {
	printf("UserListDir\n");
}

void do_UserCreate() {
	printf("UserCreate\n");
}

void do_UserRemove() {
	printf("UserRemove\n");
}

void do_UserOpen() {
	printf("UserOpen\n");
}

void do_UserClose() {
	printf("UserClose\n");
}

#else

void do_UserMkdir() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	machine->WriteRegister(2, fileSystem->Mkdir(name));
}

void do_UserRmdir() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	machine->WriteRegister(2, fileSystem->Rmdir(name));
}

void do_UserChdir() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	machine->WriteRegister(2, fileSystem->Chdir(name));
}

void do_UserListDir() {
	fileSystem->List();
}

void do_UserCreate() {
	int filenameAddr,
		size,
		type;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	size = machine->ReadRegister(5);
	type = machine->ReadRegister(6);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	machine->WriteRegister(2, fileSystem->Create(name, size, type));
}

void do_UserRemove() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	machine->WriteRegister(2, fileSystem->Remove(name));
}

void do_UserOpen() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	// TODO : return int to user
	// machine->WriteRegister(2, fileSystem->Open(name));
	machine->WriteRegister(2, 1);
}

void do_UserClose() {
	int filenameAddr;
	char name[MAX_STRING_SIZE];

	filenameAddr = machine->ReadRegister(4);
	machine->CopyStringFromMachine(filenameAddr, name, MAX_STRING_SIZE);
	// TODO : conversion name to int id
	// machine->WriteRegister(2, fileSystem->Close(name));
	machine->WriteRegister(2, 1);
}

#endif
