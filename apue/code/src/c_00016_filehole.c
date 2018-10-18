#include "apue.h"
#include <fcntl.h>

char buf[] = "abcd";

int
main(int argc, char *argv[])
{
    int fd;
    if((fd = open("tempfile", O_RDWR)) < 0) {
        printf("no such tempfile creating.\n");
        if((fd = creat("tempfile", FILE_MODE)) < 0) {
            err_sys("creat tempfile error");
        }
        printf("creat sucess\n");
    }

    if(-1 == lseek(fd, 4000000000, SEEK_SET)) {
        err_sys("lseek error");
    }

    if(4 != write(fd, buf, 4)) {
        err_sys("write error");
    }

    exit(0);
}

