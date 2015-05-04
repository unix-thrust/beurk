#!/usr/bin/python

import os,sys,commands

errors = 0
tests = 0
os.chdir(os.path.dirname(sys.argv[0]))

ROOTDIR=commands.getoutput("git rev-parse --show-toplevel").strip()

def unittest(*args):
    global errors
    global tests
    tests += 1
    cmd =  "gcc src/%s -o %s" % (args[0]+".c", args[0])
    ret = commands.getstatusoutput(cmd)
    if ret[0] == 0:
        cmd = "LD_PRELOAD=" + ROOTDIR + "/libselinux.so "
        cmd += "./" + " ".join([str(x) for x in args])
        cmd += " 2>&1"
        ret = commands.getstatusoutput(cmd)
        os.unlink(args[0])
    if ret[0] or "[BEURK_ERROR" in ret[1]:
        print("BLA")
        errors += 1
        return False
    return True

unittest("open", "test1", os.O_CREAT, 0o777)
os.unlink("test1")

# FOOTER
if errors:
    sys.exit("[-] Some tests failed (%d of %d)" % (errors, tests))
else:
    print("[+] All tests passed ! (%d)" % tests)
