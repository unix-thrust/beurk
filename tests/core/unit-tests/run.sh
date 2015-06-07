#!/usr/bin/env bash

set -o errexit
set -o verbose

cd "$(dirname $0)"
ROOTDIR=$(git rev-parse --show-toplevel)

function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

function print_good () {
    echo -e "\033[1;32m[+]\033[0;32m $1\033[0m"
}

# distclean on script exit
trap "make distclean" EXIT

# rebuild BEURK DSO with debug level 1
BEURK_DEBUG_LEVEL=1 \
    make re -C "${ROOTDIR}"

# rebuild unit-tests
make re

# use beurk DSO as library
# (using export is not a lack of compat, because we use bash on shebang...)
export LD_LIBRARY_PATH=${ROOTDIR}:${LD_LIBRARY_PATH}

# ensure that ALL source files are being tested
errors=0
for source_file in *.c; do
    [[ "$source_file" == "main.c" ]] && continue
    tested_function="${source_file%.*}"
    if ! grep -Eq '^[[:space:]]*test_'"$tested_function"'\(' "main.c"; then
    # if ! nm test | grep -Eq " T test_${tested_function}$"; then
        print_bad "function '$tested_function' is not tested !"
        (( ++errors ))
    else
        print_good "function '$tested_function' is correctly tested !"
    fi
done
[ $errors -ne 0 ] && exit 1

# run test as victim
./test

# run test as attacker
BEURK_ATTACKER=true ./test
