#include "apue.h"

int
main(int argc, char *argv[])
{
    int c;

    while(EOF != (c = getc(stdin))) {
        if(EOF == putc(c, stdout)) {
            err_sys("output error");
        }
    }

    if(ferror(stdin)) {
        err_sys("input error");
    }

    exit(0);
}

