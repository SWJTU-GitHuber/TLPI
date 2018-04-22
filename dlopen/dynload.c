#include <dlfcn.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    void *libHandle;
    void (*funcp)(void);
    const char *err;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s lib-patch func-name\n");

    libHandle = dlopen(argv[1], RTLD_LAZY);

    if (libHandle == NULL)
        fatal("dlopen:%s\n", dlerror());

    (void)dlerror();
    *(void **)(&funcp) = dlsym(libHandle, argv[2]);
    err = dlerror();
    if (err != NULL)
        fatal("dlsym:%s\n", err);

    if (funcp == NULL)
        printf("%s is NULL\n", argv[2]);
    else
        (*funcp)();

    dlclose(libHandle);
    exit(EXIT_SUCCESS);
}