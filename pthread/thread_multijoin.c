#include <pthread.h>
#include "../lib/tlpi_hdr.h"

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int totThreads = 0;
static int numLive = 0;

static int numUnjoined = 0;

enum tstate
{
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

static struct
{
    pthread_t tid;
    enum tstate state;
    int sleepTime;
} * thread;

static void *
threadFunc(void *arg)
{
    int idx = *((int *)arg);
    sleep(thread[idx].sleepTime);
    printf("Thread %d terminating\n", idx);
    pthread_mutex_lock(&threadMutex);

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    pthread_mutex_unlock(&threadMutex);

    pthread_cond_signal(&threadDied);

    return NULL;
}

int main(int argc, char *argv[])
{
    int idx;
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s nsecs...\n", argv[0]);

    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL)
        errExit("calloc");

    for (idx = 0; idx < argc - 1; idx++)
    {
        thread[idx].sleepTime = getInt(argv[idx + 1], GN_NONNEG, NULL);
        printf("idx:%d -- sleeptime:%d\n",idx,thread[idx].sleepTime);
        thread[idx].state = TS_ALIVE;
        pthread_create(&thread[idx].tid, NULL, threadFunc, &idx);
    }

    totThreads = argc - 1;
    numLive = totThreads;

    while (numLive > 0)
    {
        pthread_mutex_lock(&threadMutex);
        while (numUnjoined == 0)
        {
            pthread_cond_wait(&threadDied, &threadMutex);
        }

        for (idx = 0; idx < totThreads; idx++)
        {
            if (thread[idx].state == TS_TERMINATED)
            {
                pthread_join(thread[idx].tid, NULL);
                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                printf("Reaped thread %d (numLived=%d)\n", idx, numLive);
            }
        }
        pthread_mutex_unlock(&threadMutex);
    }

    exit(EXIT_SUCCESS);
}