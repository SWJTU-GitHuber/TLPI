#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{

    int fd = open("d.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = dup(fd);
    int fd3 = open("d.txt", O_RDWR);

    write(fd, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd, "HELLO", 6);
    write(fd3, "Giddy", 6);

    int fd4 = open("a.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    lseek(fd4, 0, SEEK_SET);
    write(fd4, "123", 3);

    close(fd);
    close(fd2);
    close(fd3);
    close(fd4);

    exit(EXIT_SUCCESS);
}

int my_dup(int fd)
{
    return fcntl(fd, F_DUPFD);
}

int my_dup2(int oldfd, int newfd)
{

    if (fcntl(oldfd, F_GETFL) == -1)
    {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd){
        return newfd;
    }

    if (fcntl(newfd, F_GETFD) != -1)
    {
        close(newfd);
        return fcntl(oldfd, F_DUPFD, newfd);
    }
    
    return -1;
}