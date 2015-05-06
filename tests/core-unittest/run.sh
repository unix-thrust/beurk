#!/bin/bash

ROOTDIR=$(git rev-parse --show-toplevel)
TESTSDIR=${ROOTDIR}/tests/core-unittest

BEURK_DEBUG_LEVEL=1 $ROOTDIR/build $ROOTDIR/beurk.conf && python $TESTSDIR/test_hook_open.py
