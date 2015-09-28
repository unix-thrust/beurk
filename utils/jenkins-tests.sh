#!/bin/bash
set -o errexit
set -o pipefail

# This script is used by jenkins.

function help() {
    echo "You need to define BEURK_VM_NAME and BEURK_VM_PORT environnements variables
before running this script."
    exit 1
}

[ -z "$BEURK_VM_NAME" ] && help
[ -z "$BEURK_VM_PORT" ] && help

# Go to the root of the project
cd /home/vagrant/project/beurk

# Disinfect the VM
sudo kill `ps -ef | grep "python -m SimpleHTTPServer 3005" | grep -v "grep" | awk -F '  +' '{print $2}'`;
sudo make disinfect && sudo make distclean

# Run the integration tests
./utils/run-tests.sh tests/quick

# Infect the VM
make re && sudo make infect
nohup -- python -m SimpleHTTPServer 3005 &

# Run the functional tests
./utils/run-tests.sh tests/functional

# Yeah!!!!
exit 0
