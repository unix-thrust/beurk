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

#include <stdio.h>
#include <stdarg.h>

#if DEBUG_LEVEL > 0
static FILE *g_debug_file = NULL;

static void init_debug(void)
{
    if (g_debug_file == NULL)
    {
        if ((g_debug_file = fopen(DEBUG_FILE, "a")) == NULL)
        {
            write(STDERR_FILENO, "\n[BEURK]: FATAL ERROR: ", 23);
            perror("cannot open DEBUG_FILE");
        }
    }
}
#else /* debug disabled */
# define DEBUG(...)
#endif

#if DEBUG_LEVEL == 1 /* debug basic */
static void debug_basic(const char *fmt, ...)
{
    va_list ap;

    init_debug();

    va_start(ap, fmt);
    fprintf(g_debug_file, "[BEURK]: ");
    vfprintf(g_debug_file, fmt, ap);
    fprintf(g_debug_file, "\n");
    va_end(ap);
}
# define DEBUG(...) (debug_basic(__VA_ARGS__))

#elif DEBUG_LEVEL == 2 /* debug high (with file and line numbers) */
static void debug_high(const char *file, int line, const char *fmt, ...)
{
    va_list ap;

    init_debug();

    va_start(ap, fmt);
    fprintf(g_debug_file, "[BEURK<%s:%d>]: ", file, line);
    vfprintf(g_debug_file, fmt, ap);
    fprintf(g_debug_file, "\n");
    va_end(ap);
}
# define DEBUG(...) (debug_high(__FILE__, __LINE__, __VA_ARGS__))
#endif
