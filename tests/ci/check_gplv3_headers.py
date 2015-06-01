#!/usr/bin/env python2

# check_gplv3_headers.py:
#   Ensure that all core sources and includes
#   include the needed BEURK's GPLv3 heaader.

import sys, commands

HEADER_STR="""/*
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
"""

sources = commands.getoutput("find src/ includes/ -name '*.c' -o -name '*.h'");
sources = [f.strip() for f in sources.splitlines() if f.strip()]

errors = 0
for filename in sources:
    with open(filename) as file:
        data = file.read()
        if not data.startswith(HEADER_STR):
            print("[-] GPL header not found in %r !" % filename)
            errors += 1

if errors:
    sys.exit(1)
