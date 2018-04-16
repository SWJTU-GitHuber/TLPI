#include <sys/wait.h>
#include "../lib/print_wait_status.h"
#include "../lib/tlpi_hdr.h"

#define MAX_CMD_LEN 200

int main(int argc, char *argv[])
{
    char str[MAX_CMD_LEN];
    int status;
    for (;;)
    {
        printf("Command: ");
        fflush(stdout);
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break;

        status = system(str);
        printf("system() retrurned: status=0x%04x (%d,%d)\n", (unsigned int)status, status >> 8, status & 0xff);
        if (status == -1)
        {
            errExit("system()");
        }
        else
        {
            if (WIFCONTINUED(status) && WEXITSTATUS(status) == 127)
                printf("(Probably) could not invoke shell\n");
            else
                printWaitStatus(NULL, status);
        }
    }
    exit(EXIT_SUCCESS);
}