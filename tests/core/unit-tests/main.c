#include <stdio.h> /* dprintf(), snprintf() */
#include <stdarg.h> /* va_list, va_start(), vdprintf() */
#include <unistd.h> /* getpid() */
#include <stdlib.h> /* setenv(), unsetenv() */
#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */

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

    // is_attacker()
    count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(is_attacker(), ++total, "is_attacker() as attacker") :
        assert_true(!is_attacker(), ++total, "is_attacker() as victim");

    // is_hidden_file()
    count += assert_true(is_hidden_file(MAGIC_STRING), ++total,
            "is_hidden_file(%s)", MAGIC_STRING);
    count += assert_true(is_hidden_file(LIBRARY_NAME), ++total,
            "is_hidden_file(%s)", LIBRARY_NAME);

    snprintf(str, 127, "%s%ld", PROC_PATH, (long)getpid());
    count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(is_hidden_file(str), ++total,
            "is_hidden_file(%s) as attacker", str) :
        assert_true(!is_hidden_file(str), ++total,
            "is_hidden_file(%s) as victim", str);

    printf("Error(s) found: %d\n", total - count);
    return ((int)total - (int)count);
}
