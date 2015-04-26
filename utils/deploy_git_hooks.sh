#! /bin/sh

ROOTDIR=$(git rev-parse --show-toplevel)

cp "${ROOTDIR}/utils/commit-msg.py" "${ROOTDIR}/.git/hooks/commit-msg"
chmod a+x "${ROOTDIR}/.git/hooks/commit-msg"
