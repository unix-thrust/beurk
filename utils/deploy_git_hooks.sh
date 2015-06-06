#!/usr/bin/env bash

ROOTDIR=$(git rev-parse --show-toplevel)

cp "${ROOTDIR}/utils/commit-msg" "${ROOTDIR}/.git/hooks/commit-msg"
chmod a+x "${ROOTDIR}/.git/hooks/commit-msg"
