#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "console.h"
#include "copyright.h"
#include "utility.h"

class SynchConsole {
public:
  // initialize the hardware console device
  SynchConsole(char *readFile, char *writeFile);

  ~SynchConsole(); // clean up console emulation

  void SynchPutChar(const char ch); // Unix putchar(3S)

  char SynchGetChar(); // Unix getchar(3S)

  void SynchPutString(const char *s); // Unix puts(3S)

  void SynchGetString(char *s, int n); // Unix fgets(3S)

  void SynchPutInt(const int n);

  int SynchGetInt();

private:
  Console *console;
};

#endif // SYNCHCONSOLE_H
