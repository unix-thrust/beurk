#!/usr/bin/env bash

# `BEURK_VM_NAME` environment variable can be overidden to
# use a specific VM for functionnal tests.
# default value: "dbian8"

set -o errexit
set -o verbose

BANNER=$(perl -E 'print "="x79 . "\r\t\t"')

cd `dirname $0`
ROOTDIR="$(git rev-parse --show-toplevel)"

if [ $# -gt 1 ]; then
    echo "Usage: $0 [VM_name]"
    exit 1
fi

BEURK_VM_PWD="/home/vagrant"
[ -z "$BEURK_VM_NAME" ] && BEURK_VM_NAME="debian8"
[ -z "$BEURK_VM_PORT" ] && BEURK_VM_PORT="5010"

function print_info () {
    echo -e "\033[1;34m[*]\033[0;36m $1\033[0m"
}
function print_good () {
    echo -e "\033[1;32m[+]\033[0;32m $1\033[0m"
}
function print_bad () {
    echo -e "\033[1;31m[-]\033[0;31m $1\033[0m"
}

function ATTACKER () {
    python "${ROOTDIR}/client.py" 127.0.0.1 $BEURK_VM_PORT 64835 b3urkR0cks -c "cd $BEURK_VM_PWD; $@"
}

function VICTIM () {
    pushd $ROOTDIR/vagrant
    vagrant ssh -c "cd $BEURK_VM_PWD; $@" "$BEURK_VM_NAME"
    popd
}

function run_functionnal_tests () {
    if [ -d "$1" ]; then
        for i in "$1"/*; do
            run_functionnal_tests "$i"
        done
    elif [ -f "$1" ] && [[ "$1" == *.test ]]; then
        print_info "$BANNER"
        print_info "RUNNING FUNCTIONNAL TEST $1 ..."
        print_info "$BANNER"
        . "$1"
        echo -e "\n"
    fi
}

run_functionnal_tests .
