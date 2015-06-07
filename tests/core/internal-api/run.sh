#!/usr/bin/env bash

set -o errexit
set -o verbose

ROOTDIR=$(git rev-parse --show-toplevel)
cd `dirname $0`

# distclean at exit
trap "make distclean || exit 1" EXIT

# rebuild test
make re

# use BEURK DSO library path
export LD_LIBRARY_PATH=${ROOTDIR}:${LD_LIBRARY_PATH}

# run test as victim
./unit-tests

# run test as attacker
BEURK_ATTACKER=true ./unit-tests
