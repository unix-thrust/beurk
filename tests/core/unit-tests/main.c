#include <sys/types.h>
#include <unistd.h>
#include "beurk.h"
#include "config.h"



int     main() {
    unsigned int    count = 0;
    char            str[128];

    if (!is_hidden_file(MAGIC_STRING))
        count++;
    if (!is_hidden_file(LIBRARY_NAME))
        count++;

    snprintf(str, 128, "%s%ld", PROC_PATH, (long)getpid());
    if (!is_hidden_file(str))
        count++;

    printf("Error(s) found: %d\n", count);
    return ((int)count);
}
