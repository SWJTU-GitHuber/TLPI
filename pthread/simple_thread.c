#include <pthread.h>
#include "../lib/tlpi_hdr.h"

static void *
threadFunc(void *argc)
{
    char *s = (char *)argc;
    printf("%s", s);
    return (void *)strlen(s);
}

void pthread_attr_demo(void)
{
    pthread_t thr;
    pthread_attr_t attr;
    int s;
    s = pthread_attr_init(&attr);
    if (s != 0)
        errExitEN(s, "pthread_attr_init");

    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (s != 0)
        errExitEN(s, "pthread_attr_setdetachstate");

    s = pthread_create(&thr, &attr, threadFunc, (void *)1);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        errExitEN(s, "pthread_attr_destroy");
}

int main(int argc, char *argr[])
{
    pthread_t t1;

    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello World\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");

    s = pthread_join(t1, &res);

    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Thread resturned %ld\n", (long)res);
    exit(EXIT_SUCCESS);
}