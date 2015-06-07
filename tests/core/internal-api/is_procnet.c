#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "tests.h" /* assert_true() */

void    test_is_procnet(unsigned int *count, unsigned int *total) {
    *count += assert_true(is_procnet(PROC_NET_TCP), ++(*total),
                "is_procnet(PROC_NET_TCP)");

    *count += assert_true(is_procnet(PROC_NET_TCP6), ++(*total),
                "is_procnet(PROC_NET_TCP6)");

    *count += assert_true(!is_procnet("marvin"), ++(*total),
                "is_procnet(\"marvin\")");
}
