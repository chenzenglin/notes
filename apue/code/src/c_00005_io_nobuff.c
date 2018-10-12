/**
 * @exmple
 * ./c_00005_io_buff < infile > outfile 
 */

#include "apue.h"

#define BUFFSIZE 4096

int
main(int argc, char *argv[])
{
    int n;
    char buf[BUFFSIZE];

    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if(n != write(STDOUT_FILENO, buf, n)) {
            err_sys("write error");
        }
    }

    if(n < 0) {
        err_sys("read error");
    }

    return 0;
}

