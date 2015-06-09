#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "hooks.h" /* open() */
#include "tests.h" /* assert_true() */

void    test_open(unsigned int *count, unsigned int *total) {
    int     fd;

    fd = open("trillian", O_CREAT, S_IRUSR);
    *count += assert_true(fd != -1, ++(*total),
            "open(\"trillian\", O_CREAT, S_IRUSR)");
    close(fd);

    fd = open("trillian", O_RDONLY);
    *count += assert_true(fd != -1, ++(*total),
            "open(\"trillian\", O_RDONLY)");
    close(fd);

    fd = open(MAGIC_STRING, O_CREAT, S_IRUSR);
    *count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(fd != -1, ++(*total),
                "open(MAGICSTRING, O_CREAT, S_IRUSR) as attacker") :
        assert_true(fd == -1, ++(*total),
                "open(MAGICSTRING, O_CREAT, S_IRUSR) as victim");
    close(fd);

    fd = open(MAGIC_STRING, O_RDONLY);
    *count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(fd != -1, ++(*total),
                "open(MAGICSTRING, O_RDONLY) as attacker") :
        assert_true(fd == -1 && errno == ENOENT, ++(*total),
                "open(MAGICSTRING, O_RDONLY) as victim");
    close(fd);
    remove(MAGIC_STRING);
    remove("trillian");
}
