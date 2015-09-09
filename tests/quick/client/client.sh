#!/bin/bash

set -ve
cd `git rev-parse --show-toplevel`

! ./client.py
./client.py host port bind_port pass missing_file.txt | grep "cannot open"
./client.py | grep "client.py <address> <port> <bind_port> \[password\] \[batch_file\]"
./client.py beurk 3005 6666 2>&1 |
    grep "host \[beurk\], port \[3005\], bind port \[6666\] : Socket error"
./client.py 127.7.7.7 3005 77777 2>&1 |
    grep "host \[127.7.7.7\], port \[3005\], bind port \[77777\] : Bind error"
./client.py 127.0.0.1 3005 64835 2>&1 |
    grep "host \[127.0.0.1\], port \[3005\], bind port \[64835\] : Connection refused"
