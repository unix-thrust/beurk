#!/usr/bin/env bash

# test: dso_functions_visibility
#
# Verify that internal API functions are NOT callable when
# DSO (libselinux.so) is compiled normally (in non debug mode)

set -o errexit
set -o verbose

cd `dirname $0`
ROOTDIR="$(git rev-parse --show-toplevel)"

trap "rm -f ./test" EXIT

function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

# ensure that all internal api function prototypes are
# set with `INTERNAL_API` macro
EXCLUDE_FILES=" init.c debug.c config.c "
for source_file in $ROOTDIR/src/*.c; do
    filename=$(basename $source_file)
    [[ "$EXCLUDE_FILES" =~ ' '$filename' ' ]] && continue
    function_name="${filename%.*}"
    grep -Eq \
        '[[:space:]]+\**'"$function_name"'\(.+\)[[:space:]]+INTERNAL_API' \
        "$ROOTDIR/includes/beurk.h" \
        || print_bad "malformed prototype '$function_name' on includes/beurk.h"
done
exit 0

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
