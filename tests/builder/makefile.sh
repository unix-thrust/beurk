#!/usr/bin/env bash

# test: makefile.sh
#
# Ensure that Makefile works as expected in
# some scenarios for building the rootkit

DSO=libselinux.so

set -ve
cd `git rev-parse --show-toplevel`


#### start with a clean repository
make distclean


#### ensure that an invalid rule fails
! make foobar


#### run make multiple times, check for `nothing to do`
make distclean
make
make all
test `make all | grep -i nothing | wc -l` -eq "1"


#### test make re
make re
test -d obj/
test -f "$DSO" -a -x "$DSO"
nm --debug-syms "$DSO" 2>&1 | grep -iq 'no symbols'
! strings "$DSO" | grep -q "_BEURK_"


#### test make re with debug mode
BEURK_DEBUG_LEVEL=1 make re
test -d obj/
test -f "$DSO" -a -x "$DSO"
! nm --debug-syms "$DSO" 2>&1 | grep -iq 'no symbols'
strings "$DSO" | grep -q "_BEURK_"


#### test make clean
make
make clean
test -f "$DSO" -a -x "$DSO"
! test -e obj/
test -e src/config.c
test -e includes/config.h


#### test make distclean
make
make distclean
! test -e "$DSO"
! test -e obj/
! test -e src/config.c
! test -e includes/config.h


#### test make infect
make distclean
! make infect
make infect 2>&1 | grep TODO


#### test make disinfect
make distclean
! make disinfect
make disinfect 2>&1 | grep TODO
