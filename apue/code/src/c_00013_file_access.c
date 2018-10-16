/**
 * ls -l a.out
 * ./a.out a.out
 * ls -l /etc/shadow
 * ./a.out /etc/shadow
 * su
 * chown root a.out
 * chmod u+s a.out
 * ls -l a.out
 * exit
 * ./a.out /etc/shadow
 */

#include "apue.h"
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    if(2 != argc) {
        err_quit("usage: a.out <pathname>");
    }
    if(access(argv[1], R_OK) < 0) {
        err_ret("access error for %s", argv[1]);
    } else {
        printf("read access OK\n");
    }

    if(open(argv[1], O_RDONLY) < 0) {
        err_ret("open error for %s", argv[1]);
    } else {
        printf("open for reading OK\n");
    }

    exit(0);
}

