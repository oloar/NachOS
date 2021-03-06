// fstest.cc
//	Simple test routines for the file system.
//
//	We implement:
//	   Copy -- copy a file from UNIX to Nachos
//	   Print -- cat the contents of a Nachos file
//	   Perftest -- a stress test for the Nachos file system
//		read and write a really large file in tiny chunks
//		(won't work on baseline system!)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "filehdr.h"
#include "filesys.h"
#include "stats.h"
#include "system.h"
#include "thread.h"
#include "utility.h"

#define TransferSize 10 // make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void Copy(const char *from, const char *to) {
  FILE *fp;
  OpenFile *openFile;
  int amountRead, fileLength;
  char *buffer;

  // Open UNIX file
  if ((fp = fopen(from, "r")) == NULL) {
    printf("Copy: couldn't open input file %s\n", from);
    return;
  }

  // Figure out length of UNIX file
  fseek(fp, 0, 2);
  fileLength = ftell(fp);
  fseek(fp, 0, 0);

  // Create a Nachos file of the same length
  DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
  if (!fileSystem->Create(to, fileLength, REGULAR)) { // Create Nachos file
    printf("Copy: couldn't create output file %s\n", to);
    fclose(fp);
    return;
  }

  openFile = fileSystem->Open(to);
  ASSERT(openFile != NULL);

  // Copy the data in TransferSize chunks
  buffer = new char[TransferSize];
  while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
    openFile->Write(buffer, amountRead);
  delete[] buffer;

  // Close the UNIX and the Nachos files
  delete openFile;
  fclose(fp);
}

//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void Print(char *name) {
  OpenFile *openFile;
  int i, amountRead;
  char *buffer;

  if ((openFile = fileSystem->Open(name)) == NULL) {
    printf("Print: unable to open file %s\n", name);
    return;
  }

  buffer = new char[TransferSize];
  while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
    for (i = 0; i < amountRead; i++)
      printf("%c", buffer[i]);
  delete[] buffer;

  delete openFile; // close the Nachos file
  return;
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName "TestFile"
#define Contents "1234567890"
#define ContentSize strlen(Contents)
#define FileSize ((int)(ContentSize * 5000))

static void FileWrite() {
  OpenFile *openFile;
  int i, numBytes;

  printf("Sequential write of %d byte file, in %zd byte chunks\n", FileSize,
         ContentSize);
  if (!fileSystem->Create(FileName, 0, REGULAR)) {
    printf("Perf test: can't create %s\n", FileName);
    return;
  }
  openFile = fileSystem->Open(FileName);
  if (openFile == NULL) {
    printf("Perf test: unable to open %s\n", FileName);
    return;
  }
  for (i = 0; i < FileSize; i += ContentSize) {
    numBytes = openFile->Write(Contents, ContentSize);
    if (numBytes < 10) {
      printf("Perf test: unable to write %s\n", FileName);
      delete openFile;
      return;
    }
  }
  delete openFile; // close file
}

static void FileRead() {
  OpenFile *openFile;
  char *buffer = new char[ContentSize];
  int i, numBytes;

  printf("Sequential read of %d byte file, in %zd byte chunks\n", FileSize,
         ContentSize);

  if ((openFile = fileSystem->Open(FileName)) == NULL) {
    printf("Perf test: unable to open file %s\n", FileName);
    delete[] buffer;
    return;
  }
  for (i = 0; i < FileSize; i += ContentSize) {
    numBytes = openFile->Read(buffer, ContentSize);
    if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
      printf("Perf test: unable to read %s\n", FileName);
      delete openFile;
      delete[] buffer;
      return;
    }
  }
  delete[] buffer;
  delete openFile; // close file
}

void PerformanceTest() {
  printf("Starting file system performance test:\n");
  stats->Print();
  FileWrite();
  FileRead();
  if (!fileSystem->Remove(FileName)) {
    printf("Perf test: unable to remove %s\n", FileName);
    return;
  }
  stats->Print();
}

void Mkdir_Test() {
  printf("\n\nMkdir Test ===============================================\n");
  printf("Creating /a/\n");
  fileSystem->Mkdir("a");
  printf("Content of / :\n");
  fileSystem->List();
  printf("Creating /b/\n");
  fileSystem->Mkdir("b");
  printf("Content of / :\n");
  fileSystem->List();
  printf("Moving into /a/\n");
  fileSystem->Chdir("a");
  printf("Creating /a/c/\n");
  fileSystem->Mkdir("c");
  printf("Content of /a :\n");
  fileSystem->List();

  printf("Cleanup...");
  fileSystem->Rmdir("c");
  fileSystem->Chdir("..");
  fileSystem->Rmdir("b");
  fileSystem->Rmdir("a");
  printf("	Content of / :\n");
  fileSystem->List();
}

void Rmdir_Test() {
  printf("\n\nRmdir Test ===============================================\n");
  printf("Creating /a/\n");
  fileSystem->Mkdir("a");
  printf("Content of / :\n");
  fileSystem->List();
  printf("Moving into /a/\n");
  fileSystem->Chdir("a");
  printf("Creating /a/b/\n");
  fileSystem->Mkdir("b");
  printf("Content of /a :\n");
  fileSystem->List();
  printf("Removing /a/b/\n");
  fileSystem->Rmdir("b");
  printf("Content of /a :\n");
  fileSystem->List();
  printf("Re-creating /a/b/\n");
  fileSystem->Mkdir("b");
  printf("Content of /a :\n");
  fileSystem->List();

  printf("Cleanup...");
  fileSystem->Rmdir("b");
  fileSystem->Chdir("..");
  fileSystem->Rmdir("a");
  printf("	Content of / :\n");
  fileSystem->List();
}

void Orig_DirectoryTest(const char *from) {
  printf("\n\nOriginal Test ============================================\n");
  printf("Creating two new directories /test1 and /test1/test2 and one file "
         "/test1/test2/TestFile:\n");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
  fileSystem->Mkdir("test1");
  printf("Contents of /:\n");
  fileSystem->List();
  fileSystem->Chdir("test1");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
  fileSystem->Mkdir("test2");
  printf("Contents of /test1:\n");
  fileSystem->List();
  fileSystem->Chdir("test2");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
  Copy(from, "TestFile");
  printf("Contents of /test1/test2:\n");
  fileSystem->List();
  fileSystem->Remove("TestFile");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
  fileSystem->Chdir("..");
  printf("Contents of /test1/test2/..:\n");
  fileSystem->List();
  fileSystem->Rmdir("test2");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
  fileSystem->Chdir("..");
  printf("Contents of /test1/test2/../..:\n");
  fileSystem->List();
  fileSystem->Rmdir("test1");
  printf("Number of open files : %d\n", OpenFile::numOpenFiles());
}

void Extra_Mkdir() {
  int i, j;
  char buff[MAX_STRING_SIZE];
  printf("\n\nExtra Mkdir Test =========================================\n");
  for (j = 0; j < 3; j++) {
    printf("Reset %d ==============================================\n", j);
    for (i = 0; i < 9; i++) {
      printf("Creating dir %d ", i);
      snprintf(buff, MAX_STRING_SIZE, "%d", i);
      if (fileSystem->Mkdir(buff))
        printf("success\n");
      else
        printf("failed\n");
    }
    for (i = 5; i < 8; i++) {
      printf("Deleting dir %d ", i);
      snprintf(buff, MAX_STRING_SIZE, "%d", i);
      if (fileSystem->Rmdir(buff))
        printf("success\n");
      else
        printf("failed\n");
    }
    for (i = 5; i < 8; i++) {
      printf("Re-Creating dir %d ", i);
      snprintf(buff, MAX_STRING_SIZE, "%d", i);
      if (fileSystem->Mkdir(buff))
        printf("success\n");
      else
        printf("failed\n");
    }
    for (i = 0; i < 8; i++) {
      printf("Deleting dir %d ", i);
      snprintf(buff, MAX_STRING_SIZE, "%d", i);
      if (fileSystem->Rmdir(buff))
        printf("success\n");
      else
        printf("failed\n");
    }
  }
}

void DirectoryTest(const char *from) {
  Mkdir_Test();
  Rmdir_Test();
  Orig_DirectoryTest(from);
  Extra_Mkdir();
  interrupt->Halt();
}
