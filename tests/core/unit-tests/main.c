#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include "beurk.h"
#include "config.h"

int     assert_true(int result, unsigned int nb, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    dprintf(2, "Test #%d: ", nb);
    vdprintf(2, fmt, args);
    if (result)
        dprintf(2, ": OK\n");
    else
        dprintf(2, ": ERROR\n");
    return (result ? 1 : 0);
}

int     main(void) {
    unsigned int    count = 0;
    unsigned int    total = 0;
    char            str[128];

    count += assert_true(is_hidden_file(MAGIC_STRING), ++total,
            "is_hidden_file(%s)", MAGIC_STRING);
    count += assert_true(is_hidden_file(LIBRARY_NAME), ++total,
            "is_hidden_file(%s)", LIBRARY_NAME);

    snprintf(str, 127, "%s%ld", PROC_PATH, (long)getpid());
    count += assert_true(is_hidden_file(str), ++total,
            "is_hidden_file(%s)", str);

    printf("Error(s) found: %d\n", count - total);
    return (!(int)count);
}
