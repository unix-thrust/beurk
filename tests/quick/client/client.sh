#!/bin/bash

set -ve
cd `git rev-parse --show-toplevel`

# Ensure that client launched without arguments returns nonzero
! ./client.py

# Try to pass an invalid file as batch file
./client.py host port bind_port pass missing_file.txt | grep "cannot open"

# Ensure running client without arguments prints the correct help message
./client.py | grep "client.py <address> <port> <bind_port> \[password\] \[batch_file | -c command\]"

# Check socket error
./client.py beurk 3005 6666 2>&1 |
    grep "host \[beurk\], port \[3005\], bind port \[6666\] : \[Errno -2\] Name or service not known"
./client.py 777.777.777.777 3005 6666 2>&1 |
    grep "host \[777.777.777.777\], port \[3005\], bind port \[6666\] : \[Errno -2\] Name or service not known"

# Check connection error
./client.py 127.0.0.1 3005 64835 2>&1 |
    grep "host \[127.0.0.1\], port \[3005\], bind port \[64835\] : \[Errno 111\] Connection refused"

# Check bind error
if python --version 2>&1 | grep -Eq '^Python 2\.7'; then
    # for 2.7
    ./client.py 127.0.0.1 3005 77777 2>&1 |
        grep "host \[127.0.0.1\], port \[3005\], bind port \[77777\] : getsockaddrarg: port must be 0-65535."
fi
