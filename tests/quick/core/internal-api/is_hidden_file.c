#include <stdio.h> /* dprintf(), snprintf() */
#include <unistd.h> /* getpid() */
#include <stdlib.h> /* getenv() */
#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "tests.h"

void    test_is_hidden_file(unsigned int *count, unsigned int *total) {
    char str[128];

    *count += assert_true(!is_hidden_file("zaphod"), ++(*total),
            "is_hidden_file(%s)", "\"zaphod\"");
    *count += assert_true(is_hidden_file(MAGIC_STRING), ++(*total),
            "is_hidden_file(%s)", MAGIC_STRING);
    *count += assert_true(is_hidden_file(LIBRARY_NAME), ++(*total),
            "is_hidden_file(%s)", LIBRARY_NAME);

    snprintf(str, 128, "%s%ld", PROC_PATH, (long)getpid());
    *count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(is_hidden_file(str), ++(*total),
            "is_hidden_file(%s) as attacker", str) :
        assert_true(!is_hidden_file(str), ++(*total),
            "is_hidden_file(%s) as victim", str);
}
