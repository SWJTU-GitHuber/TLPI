#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"
#include <dirent.h>
#include <ftw.h>
#include <unistd.h>

#define CMD_SIZE 200
#define BUF_SIZE 1024
#define _XOPEN_SOURCE 600

static void listFiles(const char *);
static void dirLink(const int argc, const char *argv[]);

int main(int argc, char *argv[])
{

    char cwd[256];
    char* s = getcwd(cwd, 256);
    printf("Current work dir: %s\n", s);

    exit(EXIT_SUCCESS);
}

static void
dirLink(const int argc, const char *argv[])
{
    int fd, j, numBlocks;
    char shellCmd[CMD_SIZE];
    char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s temp-file [num-1kB-blocks] \n", argv[0]);
    }

    numBlocks = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-1kB-blocks") : 100000;

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    if (fd == -1)
        errExit("open");

    if (unlink(argv[1]) == -1)
        errExit("unlink");

    for (j = 0; j < numBlocks; j++)
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
            fatal("partial/failed write");

    snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`s", argv[1]);
    system(shellCmd);

    if (close(fd) == -1)
        errExit("close");

    printf("***************Closed file descriptor\n");

    system(shellCmd);
}

static void
listFiles(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp;
    Boolean isCurent;

    isCurent = strcmp(dirpath, ".");

    dirp = opendir(dirpath);
    if (dirp == NULL)
    {
        errMsg("opendir failed '%s", dirpath);
        return;
    }

    for (;;)
    {
        errno = 0;
        dp = readdir(dirp);

        if (dp == NULL)
            break;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        if (!isCurent)
            printf("%s/", dirpath);

        printf("%s\n", dp->d_name);
    }

    if (errno != 0)
        errExit("readdir");

    if (closedir(dirp) == -1)
        errExit("closedir");
}

static void usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    fprintf(stderr, "\t-p Use FTW_PHYS flag \n");
    exit(EXIT_FAILURE);
}
struct FTW{
    int base;
    int level;
};

static int
didTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{

    switch (sbuf->st_mode & S_IFMT)
    {
    case S_IFREG:
        printf("-");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    default:
        printf("?");
        break;
    }

    printf(" %s ",
           (type == FTW_D) ? "D" : (type == FTW_DNR) ? "DNR" : (type == FTW_D) ? "DP" : (type == FTW_F) ? "F" : (type == FTW_SL) ? "SL" : (type == FTW_NS) ? "NS" : " ");

    if(type != FTW_NS)
        printf("%7ld",(long)sbuf->st_ino);
    else
        printf("    ");

    printf(" %*s", 4* ftwb->level, "");
    printf("%s\n", &pathname[ftwb->base]);
    return 0;
}