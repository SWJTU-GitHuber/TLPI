#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{

    int fd;
    struct iovec iov[3];

    char buffer1[5]; //First buffer

    char buffer2[5]; //Second buffer

    char buffer3[5]; //Third buffer
    
    printf("buffer1 addr: %p, buffer2 addr: %p, buffer3 addr: %p\n", buffer1, buffer2, buffer3);

    ssize_t numRead, totRequired;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s file\n", argv[0]);
    }

    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
    {
        errExit("readv open");
    }

    totRequired = 0;

    iov[0].iov_base = &buffer1;
    iov[0].iov_len = sizeof(buffer1);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &buffer2;
    iov[1].iov_len = sizeof(buffer2);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = &buffer3;
    iov[2].iov_len = sizeof(buffer3);
    totRequired += iov[2].iov_len;

    numRead = readv(fd, iov, 3);
    if (numRead == -1)
        errExit("readv");

    printf("readv lenght: %d\n", (int)numRead);

    if (numRead < totRequired)
        printf("Read fewer bytes than requested\n");

    printf("buffer1 addr: %p, buffer2 addr: %p, buffer3 addr: %p\n", buffer1, buffer2, buffer3);
    printf("total bytes requested: %ld; bytes read: %ld %s -- %s -- %s\n", (long)totRequired, (long)numRead, buffer1, buffer2, buffer3); //buffer1/buffer2/buffer3 are consequent

    int fd_w = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fd_w == -1)
    {
        errExit("writev open");
    }

    struct iovec src[3];

    src[0].iov_base = &buffer1;
    src[0].iov_len = sizeof(buffer1);

    src[1].iov_base = &buffer2;
    src[1].iov_len = sizeof(buffer2);

    src[2].iov_base = &buffer3;
    src[2].iov_len = sizeof(buffer3);

    ssize_t numWrite = writev(fd_w, src, 3);
    if (numWrite < numRead)
    {
        printf("already write data is less than we Read -Num: %d\n", (int)numWrite);
    }

    exit(EXIT_SUCCESS);
}