#!/usr/bin/env bash

set -o errexit
set -o verbose

function print_good () {
    echo -e "\033[1;32m[+]\033[0;32m $1\033[0m"
}
function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

# use gmake on freebsd
[ `uname -s` = "FreeBSD" ] && MAKE=gmake || MAKE=make

# set ROOTDIR and current dir
ROOTDIR=$(git rev-parse --show-toplevel)
cd `dirname $0`

# ensure that a .c file exists for each internal api source file
# e.g: src/is_attacker.c MUST have a corresponding .c file in current dir
EXCLUDE_FILES=" init.c debug.c config.c "
for file in $(find $ROOTDIR/src/ -maxdepth 1 -type f -name '*.c' -exec basename {} \;); do
    [[ "$EXCLUDE_FILES" =~ ' '$file' ' ]] && continue
    func="test_${file%.*}"
    # ensure internal-api source file has a test dedicated file
    if [ ! -f "$file" ]; then
        print_bad "missing test file: '$file' !"
        (( ++errors ))
    # ensure test function is called in main.c
    elif ! grep -Eq '^[[:space:]]*'"$func"'\(' main.c; then
        print_bad "'$func' is not called in main.c !"
        (( ++errors ))
    else
        print_good "'$file' OK"
    fi
done

[ $errors -ne 0 ] && exit 1

# distclean at exit
trap "${MAKE} distclean" EXIT

# rebuild test
${MAKE} re

# use BEURK DSO library path
export LD_LIBRARY_PATH=${ROOTDIR}:${LD_LIBRARY_PATH}

# run test as victim
./unit-tests

# run test as attacker
BEURK_ATTACKER=true ./unit-tests
