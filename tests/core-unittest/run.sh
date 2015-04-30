#!/bin/sh

ROOTDIR=$(git rev-parse --show-toplevel)
TESTSDIR=${ROOTDIR}/tests/core-unittest

$ROOTDIR/build $ROOTDIR/beurk.conf && sh $TESTSDIR/test_hook_open.sh
