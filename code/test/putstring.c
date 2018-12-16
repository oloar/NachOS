#include "syscall.h"

#define RANGE 26

void printstr(char c) {
  char buff[RANGE + 2];
  int i;

  for (i = 0; i < RANGE; i++)
    buff[i] = c + i;
  buff[RANGE] = '\n';
  buff[RANGE + 1] = '\0';

  PutString(buff);
}

int main() {
  printstr('a');
  return 0;
}
