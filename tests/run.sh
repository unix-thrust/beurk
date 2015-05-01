#!/bin/bash

ROOTDIR=$(git rev-parse --show-toplevel)
TESTDIR=${ROOTDIR}/tests
array="ci core-unittest"
err_count=0

for element in ${array}
do
  ${TESTDIR}/$element/run.sh
  err_count=$(($err_count+$?))
done
if [ $err_count -gt 0 ]; then
  exit 1
fi
