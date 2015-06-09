#!/usr/bin/env python2

# ensure that hooked functions are NEVER used internally
# (this errors leads to stack overflows most of the time)

import os
import sys
import re
import commands

from pprint import pprint

# chdir to git root directory
os.chdir(commands.getoutput("git rev-parse --show-toplevel").strip())

# compile BEURK for hooks testing
if os.uname()[0] == 'FreeBSD':
    buildcmd = "BEURK_DEBUG_LEVEL=1 gmake re"
else:
    buildcmd = "BEURK_DEBUG_LEVEL=1 make re"
print("$ " + buildcmd)
assert os.system(buildcmd) == 0

# get all hooked functions
hooked_functions = []
regex = re.compile("^.+\\b(\w+)\(.*\)\s*;\s*$")
header = open("includes/hooks.h").read()
header = header.replace("\\\n", "").splitlines()
header = [l for l in header if not l.startswith("#")]
for line in header:
    try:
        match = list(regex.match(line).groups())
        assert len(match) == 1
    except:
        continue
    hooked_functions.append(match[0])

# check if some hooks are used internally (which is illegal)
ERRORS = 0
for hook in hooked_functions:
    cmd = "find . -type f -name '*.o' -exec nm {} ';' | grep -E ' U %s$' > /dev/null"
    print("$ " + cmd % hook)
    if os.system(cmd % hook) == 0:
        print("--------------------------")
        print("ERROR: hooked function `%s` is used internally:" % hook)
        os.system("find . -name '*.c' -o -name '*.h' | xargs grep --color=always -n '%s('" % hook)
        print("--------------------------")
        print("")
        ERRORS += 1
    else:
        print("OK")

if ERRORS > 0:
    exit(1)
