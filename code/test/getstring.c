#include "syscall.h"

int main() {
  char buff[256];

  GetString(buff, 256);
  PutString(buff);
  GetString(buff, 256);
  PutString(buff);
  return 0;
}
