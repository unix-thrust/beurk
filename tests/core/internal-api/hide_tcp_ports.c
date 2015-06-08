#include <string.h> /* strcmp() */
#include <stdio.h> /* fopen() */
#include "beurk.h" /* all tested functions */
#include "config.h" /* all literals */
#include "tests.h" /* assert_true() */

#define BUF_SIZE 24000
void    test_hide_tcp_ports(unsigned int *count, unsigned int *total) {

    char buf_1[BUF_SIZE] = {0};
    FILE *file_1 = REAL_FOPEN("./hide_tcp_ports/proc-net-tcp-without-hidden_port.txt", "r");
    fread(buf_1, BUF_SIZE, 1, file_1);

    char buf_2[BUF_SIZE] = {0};
    FILE *file_2 = fopen("hide_tcp_ports/proc-net-tcp-with-hidden_port.txt", "r");
    fread(buf_2, BUF_SIZE, 1, file_2);

    FILE *_file_1 = hide_tcp_ports("hide_tcp_ports/proc-net-tcp-without-hidden_port.txt");
    char _buf_1[BUF_SIZE] = {0};
    fread(_buf_1, BUF_SIZE, 1, _file_1);

    FILE *_file_2 = hide_tcp_ports("hide_tcp_ports/proc-net-tcp-with-hidden_port.txt");
    char _buf_2[BUF_SIZE] = {0};
    fread(_buf_2, BUF_SIZE, 1, _file_2);

    *count += assert_true(*buf_1,
            ++(*total), "check that buf_1 is not empty");

    *count += assert_true(strcmp(buf_1, buf_2) != 0,
            ++(*total), "fopen(non-hidden-procnet) != fopen(hidden-procnet)");

    *count += assert_true(strcmp(buf_1, _buf_1) == 0,
            ++(*total), "fopen(non-hidden-procnet) == hide_tcp_ports(non-hidden-procnet)");

    *count += assert_true(strcmp(_buf_1, _buf_2) == 0,
            ++(*total), "hide_tcp_ports(hidden-procnet) == hide_tcp_ports(non-hidden-procnet)");

    *count += assert_true(strcmp(buf_1, _buf_2) == 0,
            ++(*total), "fopen(non-hidden-procnet) == hide_tcp_ports(hidden-procnet)");
}
