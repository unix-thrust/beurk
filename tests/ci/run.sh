#!/bin/bash

ROOTDIR=$(git rev-parse --show-toplevel)

${ROOTDIR}/tests/ci/check_commits_history.sh
exit $?
