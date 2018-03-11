#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "../lib/tlpi_hdr.h"
#include "../lib/get_num.h"

#define MAX_LENGTH 1024

int main(int argc, char *argv[])
{

    int srcFd, dstFd;
    char buffer[MAX_LENGTH];

    char *a = "abcdef";//6
    char *b = "ghijk";//5
    char *c = "lmn";//3
    char *d = "op";//2

    off_t hole1_t = 8, hole2_t = 16, hole3_t = 23;

    printf("arg1: %s -- arg2: %s\n", argv[1], argv[2]);

    srcFd = open(argv[1], O_RDWR | O_CREAT /*O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWOTH*/);
    if (srcFd == -1)
    {
        errExit("open falied");
    }

    write(srcFd, a, strlen(a));//6
    lseek(srcFd, hole1_t, SEEK_SET);//2
    write(srcFd, b, strlen(b));
    lseek(srcFd, hole2_t, SEEK_SET);
    write(srcFd, c, strlen(c));
    lseek(srcFd, hole3_t, SEEK_SET);
    write(srcFd,d,strlen(d));//test for del?

    dstFd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWOTH);
    if (dstFd == -1)
    {
        errExit("open falied");
    }

    lseek(srcFd, 0, SEEK_SET);

    int readLen = read(srcFd, buffer, MAX_LENGTH);
    if (readLen == -1)
    {
        errExit("read failed");
    }

    printf("readLen: %d\n", readLen);

    int writeLen = 0;

    writeLen = write(dstFd, buffer, readLen);
    printf("writeLen: %d -- info: %s\n", writeLen, buffer);
    if (writeLen < readLen)
        err_exit("write data error\n");

    if (close(srcFd) == -1)
        err_exit("close failed");
    if (close(dstFd) == -1)
        err_exit("open failed");
}