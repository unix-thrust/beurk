#!/usr/bin/python

import os,sys,commands

os.chdir(os.path.dirname(sys.argv[0]))
sys.path.insert(0, os.path.abspath("src"))
from unittest import *

unittest("call open(2) hooked", "open", "test1", os.O_CREAT, 0o777)
os.unlink("test1")
unittest("call open(2) hooked", "open", "test1", os.O_RDONLY)

# FOOTER
if errors:
    sys.exit("[-] Some tests failed (%d of %d)" % (unittest_errors(), unittest_tests()))
else:
    print("[+] All tests passed ! (%d)" % unittest_tests())
