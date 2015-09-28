#!/bin/bash

# This script is used by jenkins.

# Go to the root of the project
cd /home/vagrant/project/beurk

# Disinfect the VM
BEURK_PID=`ps -ef | grep "python -m SimpleHTTPServer 3005" | grep -v "grep" | awk -F '  +' '{print $2}'`
for PID in "$BEURK_PID"; do
    echo $PID
    sudo kill -9 $PID
done

sudo make disinfect && sudo make distclean
unset BEURK_PID

# Run the integration tests
./utils/run-tests.sh tests/quick

# Infect the VM
make re && sudo make infect
sudo python -m SimpleHTTPServer 3005 > /dev/null &

# Yeah!!!!
exit 0
