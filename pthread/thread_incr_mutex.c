#include <pthread.h>
#include "../lib/tlpi_hdr.h"

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *
threadFunc(void *arg)
{
    int loops = *((int *)arg);
    int loc, j, s;

    for (j = 0; j < loops; j++)
    {
        s = pthread_mutex_lock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");
        loc = glob;
        loc++;
        glob = loc;
        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t t1, t2;
    int loops, s;

    loops = 30;

    pthread_create(&t1, NULL, threadFunc, &loops);

    pthread_create(&t2, NULL, threadFunc, &loops);

    pthread_join(t1, NULL);

    pthread_join(t2, NULL);
    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}