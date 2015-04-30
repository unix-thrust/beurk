#!/bin/sh

ROOTDIR=$(git rev-parse --show-toplevel)
TESTDIR=${ROOTDIR}/tests

${TESTDIR}/core-unittest/run.sh
