#!/bin/bash

# test: check_needed_unit_tests.sh
#
# Ensure that all hooks and internal API functions
# have a dedicated unit-test.

# C files to ignore in src/
# all other files will be checked for a matching unit test
EXCLUDE_FILES=" init.c debug.c config.c "

# location for unit test .c files
# (the same filename is expected than for sources)
UNIT_TEST_DIR="./tests/core/unit-tests"

cd `git rev-parse --show-toplevel` || exit 1

function print_good () {
    echo -e "\033[1;32m[+]\033[0;32m $1\033[0m"
}
function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

errors=0
while read filename; do
    [[ "$EXCLUDE_FILES" =~ ' '$filename' ' ]] && continue
    if [ -f "$UNIT_TEST_DIR/$filename" ]; then
        print_good "$filename unit-test is available. "
    else
        print_bad "$filename unit-test is missing! "
        (( ++errors ))
    fi
done < <(ls -1 src/*.c | cut -d'/' -f2)

[ $errors -ne 0 ] && exit 1
