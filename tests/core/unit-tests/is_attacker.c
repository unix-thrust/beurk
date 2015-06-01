#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "tests.h" /* assert_true() */

void    test_is_attacker(unsigned int *count, unsigned int *total) {
    *count += getenv(HIDDEN_ENV_VAR) != NULL ?
        assert_true(is_attacker(), ++(*total), "is_attacker() as attacker") :
        assert_true(!is_attacker(), ++(*total), "is_attacker() as victim");
}
