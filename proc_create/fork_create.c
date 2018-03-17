#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../lib/tlpi_hdr.h"

static void parent_fchile_share()
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    setbuf(stdout, NULL);

    fd = mkstemp(template);
    if (fd == -1)
        errExit("mkstmp");

    printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl - F_GETFL");

    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork())
    {
    case -1:
        errExit("fork");
        break;
    case 0:
        if (lseek(fd, 1000, SEEK_SET) == -1)
            errExit("lseek");

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            errExit("fcntl - G_GETFL");

        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1)
            errExit("fcntl - F_SETFL");
        _exit(EXIT_SUCCESS);
        break;
    default:
        if (wait(NULL) == -1)
            errExit("wait");
        printf("Child has exited\n");

        printf("File offset in parent: %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            errExit("fcntl - FGETFL");
        printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
        exit(EXIT_SUCCESS);
        break;
    }
}

static void handler(int sig) {}

#define SYNC_SIG SIGUSR1

static void fork_avoid_race()
{
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork())
    {
    case -1:
        errExit("fork");
        break;

    case 0:
        printf("[%ld] Child started - doing some work\n", (long)getpid());
        sleep(2);

        printf("[%ld] Child about to signal parent\n", (long)getpid());

        if (kill(getpid(), SYNC_SIG) == -1)
            errExit("kill");

        _exit(EXIT_SUCCESS);
        break;

    default:
        printf("[%ld] Parent about to wait for signal\n", (long)getpid());
        sigemptyset(&emptyMask);
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend");

        printf("[%ld] Parent got signal\n", (long)getpid());

        if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
            errExit("sigprocmask");

        exit(EXIT_SUCCESS);
        break;
    }
}

int main(int argc, char *argv[])
{
    fork_avoid_race();
}