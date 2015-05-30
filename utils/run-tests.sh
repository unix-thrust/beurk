#!/bin/bash

# run-tests.sh:
#   Run a test script or all tests contained in
#   a given directory, in a recursive manner.

TESTDIR="tests"

BANNER=$(perl -E 'print "="x79 . "\r\t\t"')

function print_info () {
    echo -e "\033[1;34m[*]\033[0;36m $1\033[0m"
}
function print_good () {
    echo -e "\033[1;32m[+]\033[0;32m $1\033[0m"
}
function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

function usage () {
    echo "$0:"
    echo "  Run a test script or all tests contained in"
    echo "  a given directory, in a recursive manner."
    echo ""
    echo "Usage: $0 <tests-file>"
    echo "       $0 <tests-directory>"
    exit 1
}

[ $# -eq 1 ] || usage

tests=0
errors=0
while read test_script; do
    print_info "$BANNER"
    print_info "RUNNING $test_script ..."
    print_info "$BANNER"
    if "$test_script"; then
        print_good "$test_script succeeded"
    else
        print_bad "$test_script failed !"
        (( ++errors ))
    fi
    echo -e "\n"
    (( ++tests ))
done < <(find "$1" -type f -perm +111 | grep "${TESTDIR}/")

[ $tests -gt 0 ] || usage

echo
if [ $errors -eq 0 ]; then
    print_info "$BANNER TESTS SUMMARY "
    print_info "All tests ($tests) succeeded! "
    print_info "$BANNER\n"
    exit 0
else
    print_bad "$BANNER TESTS SUMMARY "
    print_bad "Some tests ($errors/$tests) failed! "
    print_bad "$BANNER\n"
    exit 1
fi
