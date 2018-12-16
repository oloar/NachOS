#include "syscall.h"

int main() {
  char c = GetChar();

  PutString("Le caractère entré est : ");
  PutChar(c);
  PutChar('\n');
  return 0;
}
