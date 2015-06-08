#pragma once

#ifndef _TESTS_H_
# define _TESTS_H_

#include <stdarg.h> /* va_list, va_start(), vdprintf() */

int     assert_true(int result, unsigned int nb, const char *fmt, ...);
void    test_is_attacker(unsigned int *count, unsigned int *total);
void    test_is_hidden_file(unsigned int *count, unsigned int *total);
void    test_is_procnet(unsigned int *count, unsigned int *total);
void    test_open(unsigned int *count, unsigned int *total);
void    test_hide_tcp_ports(unsigned int *count, unsigned int *total);
void    test_drop_shell_backdoor(unsigned int *count, unsigned int *total);
void    test_cleanup_login_records(unsigned int *count, unsigned int *total);

#endif /* _TESTS_H_ */
