#!/usr/bin/env bash

set -o errexit
set -o verbose

if [ `uname -s` = "FreeBSD" ]; then
    MAKE=gmake
else
    MAKE=make
fi

ROOTDIR=$(git rev-parse --show-toplevel)

UNIT_TESTS="tests/core/internal-api"

$MAKE -C ${ROOTDIR}/${UNIT_TESTS}
LD_LIBRARY_PATH=${ROOTDIR}:$LD_LIBRARY_PATH ${ROOTDIR}/${UNIT_TESTS}/unit-tests
BEURK_ATTACKER=true LD_LIBRARY_PATH=${ROOTDIR}:$LD_LIBRARY_PATH \
    ${ROOTDIR}/${UNIT_TESTS}/unit-tests
$MAKE distclean -C ${ROOTDIR}/${UNIT_TESTS}
