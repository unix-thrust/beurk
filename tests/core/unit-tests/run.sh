#! /bin/bash
ROOTDIR=$(git rev-parse --show-toplevel)

UNIT_TESTS="tests/core/unit-tests"

make -C ${ROOTDIR}/${UNIT_TESTS}
LD_LIBRARY_PATH=${ROOTDIR}:$LD_LIBRARY_PATH ${ROOTDIR}/${UNIT_TESTS}/libtest
