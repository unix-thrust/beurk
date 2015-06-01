#!/bin/bash

set -ve
cd `git rev-parse --show-toplevel`

test ./utils/client.sh | grep TODO 

