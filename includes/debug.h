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

#ifndef _DEBUG_H_
# define _DEBUG_H_

# if DEBUG_LEVEL > 0
typedef enum    dbg_type {
    D_INFO,
    D_ERROR,
}               e_dbg_type;

# else /* debug disabled */
#  define DEBUG(...) ((void)0)
# endif

# if DEBUG_LEVEL == 1 /* debug basic */
void            debug(e_dbg_type flag, const char *fmt, ...);
#  define DEBUG(...) (debug(__VA_ARGS__))

# elif DEBUG_LEVEL == 2 /* debug high (with file and line numbers) */
void            debug(const char *f, int l, e_dbg_type flag, char *fmt, ...);
#  define DEBUG(...) (debug(__FILE__, __LINE__, __VA_ARGS__))

# endif /* DEBUG_LEVEL */

#endif /* _DEBUG_H_ */
