#!/usr/bin/env bash

ROOTDIR=$(git rev-parse --show-toplevel)
count=0
err_count=0

while $(git log HEAD~$count -n 1 --no-merges --pretty="%B" &> /dev/null);
do
  git log HEAD~$count -n 1 --no-merges --pretty="%B" \
      | python2 $ROOTDIR/utils/commit-msg.py
  err_count=`expr "$err_count" + "$?"`
  count=`expr "$count" + "1"`
done
if [ $err_count -gt 0 ]; then
  exit 1
fi
