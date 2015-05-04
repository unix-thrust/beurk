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
    print cmd
    ret = commands.getstatusoutput(cmd)
    print ret
    if ret[0] == 0:
        cmd = "LD_PRELOAD=" + ROOTDIR + "/libselinux.so "
        cmd += "./" + " ".join([str(x) for x in args])
        cmd += " 2>&1"
        print cmd
        ret = commands.getstatusoutput(cmd)
        os.unlink(args[0])
    if ret[0]:
        errors += 1
    return False if "[BEURK_ERROR" in ret[1] else True

unittest("open", "test1", os.O_CREAT, 0o777)
os.unlink("test1")






# END
if errors:
    sys.exit("[-] Some tests failed (%d of %d)" % (errors, tests))
else:
    print("[+] All tests passed ! (%d)" % tests)
