#!/bin/bash

set -ve
cd `git rev-parse --show-toplevel`

# Ensure that client launched without arguments returns nonzero
! ./client.py

# Try to pass an invalid file as batch file
./client.py host port bind_port pass missing_file.txt | grep "cannot open"

# Ensure running client without arguments prints the correct help message
./client.py | grep "client.py <address> <port> <bind_port> \[password\] \[batch_file | -c command\]"

# Check 'Socket error' output error
./client.py beurk 3005 6666 2>&1 |
    grep "host \[beurk\], port \[3005\], bind port \[6666\] : Socket error"

# Check 'Connexion refused' output error
./client.py 127.0.0.1 3005 64835 2>&1 |
    grep "host \[127.0.0.1\], port \[3005\], bind port \[64835\] : Connection refused"

# As we were unable to distinguish "Bind Error" from "Connection refused"
# errors with python 2.6, this test is disabled for this version of python
if python --version 2>&1 | grep -Eqv '^Python 2\.6'; then
    # check 'Bind Error' output error
    ./client.py 127.7.7.7 3005 77777 2>&1 |
        grep "host \[127.7.7.7\], port \[3005\], bind port \[77777\] : Bind error"
fi
