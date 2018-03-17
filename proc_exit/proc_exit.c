#define _BSD_SOURCE
#include <stdlib.h>
#include "../lib/tlpi_hdr.h"

static void
atexitFunc1(void)
{
    printf("atexit function 1 called\n");
}

static void
atexitFunc2(void)
{
    printf("atexit function 2 called\n");
}

static void
ontexitFunc2(int status, void *arg)
{
    printf("onexit function called:status=%d, arg=%ld\n", status, (long)arg);
}

int main(int argc, char *argv[])
{
    if (on_exit(ontexitFunc2, (void *)10) != 0)
        errExit("on_exit");

    if (atexit(atexitFunc1) != 0)
        errExit("atexitFunc1");

    if (atexit(atexitFunc2) != 0)
        errExit("atexitFunc2");

    if (on_exit(ontexitFunc2, (void *)100) != 0)
        errExit("on_exit");
        
    exit(2);
}