#!/bin/bash
set -o errexit
set -o pipefail

# This script is used by jenkins.

# Go to the root of the project
cd /home/vagrant/project/beurk

# Disinfect the VM
BEURK_PID=`ps -ef | grep "python -m SimpleHTTPServer 3005" | grep -v "grep" | awk -F '  +' '{print $2}'`
[ -z "$BEURK_PID" ] && sudo kill $BEURK_PID
sudo make disinfect && sudo make distclean

# Run the integration tests
./utils/run-tests.sh tests/quick

# Infect the VM
make re && sudo make infect
nohup -- python -m SimpleHTTPServer 3005 &

# Yeah!!!!
exit 0
