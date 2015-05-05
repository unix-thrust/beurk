import os,sys,commands
ROOTDIR=commands.getoutput("git rev-parse --show-toplevel").strip()

errors = 0
tests = 0

def unittest_errors():
    return errors

def unittest_tests():
    return tests


def unittest(*args):
    global errors
    global tests
    tests += 1
    cmd =  "gcc src/%s -o %s" % (args[1]+".c", args[1])
    ret = commands.getstatusoutput(cmd)
    if ret[0] == 0:
        cmd = "LD_PRELOAD=" + ROOTDIR + "/libselinux.so "
        cmd += "./" + " ".join([str(x) for x in args[1:]])
        cmd += " 2>&1"
        ret = commands.getstatusoutput(cmd)
        os.unlink(args[1])
    if ret[0] or "[BEURK_ERROR" in ret[1]:
        errors += 1
        return False
    elif "[BEURK_INFO]: " + args[0] in ret[1]:
        print ret[1]
        return True
    else:
        return False

