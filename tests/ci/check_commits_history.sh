ROOTDIR=$(git rev-parse --show-toplevel)
count=0

while git log HEAD~$count -n 1 --pretty="%B" &> /dev/null
do
  git log HEAD~$count -n 1 --pretty="%B"
  git log HEAD~$count -n 1 --pretty="%B" \
      | python2 $ROOTDIR/utils/commit-msg.py
  (( count++ ))
done
