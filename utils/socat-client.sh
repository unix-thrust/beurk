#!/usr/bin/env bash

if [ $# -eq 2 ]; then
    printf "BEURK password: "; socat -,raw,echo=0 TCP:$1,bind=:$2
else
    echo "Usage: $0 <addr:port> <bind_port>"
fi
