#include <stdio.h> /* dprintf(), snprintf() */
#include <stdarg.h> /* va_list, va_start(), vdprintf() */
#include "tests.h"

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

    test_is_attacker(&count, &total);
    test_is_hidden_file(&count, &total);
    test_is_procnet(&count, &total);
    test_open(&count, &total);
    test_hide_tcp_ports(&count, &total);
    test_drop_shell_backdoor(&count, &total);
    test_cleanup_login_records(&count, &total);

    printf("Error(s) found: %d\n", total - count);
    return ((int)total - (int)count);
}
