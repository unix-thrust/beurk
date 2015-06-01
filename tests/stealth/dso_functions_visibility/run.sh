#!/bin/bash

# test: dso_functions_visibility
#
# Verify that internal API functions are NOT callable when
# DSO (libselinux.so) is compiled normally (in non debug mode)

set -ve
cd `dirname $0`
ROOTDIR="$(git rev-parse --show-toplevel)"

trap "rm -f ./test" EXIT


# ensure that without debug mode, compilation fails with
# something like `undefined reference to is_hidden_file()`
BEURK_DEBUG_LEVEL=0 make re -C "${ROOTDIR}"
cc -I${ROOTDIR}/includes -L${ROOTDIR} -lselinux test.c -o test 2>&1 \
    | grep -i 'undefined reference' | grep "is_hidden_file"


# ensure that with debug level 1, compilation doesn't fail, and
# `called fn()` beurk debug lines are here
BEURK_DEBUG_LEVEL=1 make re -C "${ROOTDIR}"
cc -I${ROOTDIR}/includes test.c -L${ROOTDIR} -lselinux -o test
LD_PRELOAD=${ROOTDIR}/libselinux.so ./test 2>&1 \
    | grep -i "called is_hidden_file()"


# ensure that with debug level 2, compilation doesn't fail, and
# `called fn()` beurk debug lines are here
BEURK_DEBUG_LEVEL=2 make re -C "${ROOTDIR}"
cc -I${ROOTDIR}/includes test.c -L${ROOTDIR} -lselinux -o test
LD_PRELOAD=${ROOTDIR}/libselinux.so ./test 2>&1 \
    | grep -i "called is_hidden_file()"
