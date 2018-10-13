#include "apue.h"
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while(NULL != fgets(buf, MAXLINE, stdin)) {
        if('\n' == buf[strlen(buf) - 1]) {
            buf[strlen(buf) -1] = 0;
        }
        if((pid = fork()) < 0) {
            err_sys("fork error");
        } else if(0 == pid) { //child
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execte: %s", buf);
            exit(127);
        }

        //parent
        if((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("%% ");
    }

    exit(0);
}

