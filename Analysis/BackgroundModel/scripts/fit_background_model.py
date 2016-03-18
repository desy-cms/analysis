#!/usr/bin/env python

import sys
import ast
import fcntl
import argparse
import subprocess
import time
import datetime


################################################################################
def main(argv=None):
    """
    Main routine which is not called, if this module is loaded via `import`.

    Arguments:
    - `argv`: command line arguments passed to the script
    """

    if argv == None:
        argv = sys.argv[1:]

    parser = argparse.ArgumentParser(
        description = "Perform background model studies.")
    parser.add_argument("--list", dest = "list", required = True,
                        help = ("Python list representation containing the "
                                "options to be passed to 'FitBackgroundModel'."))
    parser.add_argument("--success", dest = "success", required = True,
                        help = "Target file containing successful configs.")
    parser.add_argument("--fail", dest = "fail", required = True,
                        help = "Target file containing failed configs.")
    args = parser.parse_args(argv)

    arguments_list = ast.literal_eval(args.list)

    if len(arguments_list) > 0 and type(arguments_list[0]) not in (tuple, list):
        FitBackgroundModel(args.success, args.fail, *arguments_list)
    else:
        map(lambda x: FitBackgroundModel(args.success, args.fail, *x),
            arguments_list)




################################################################################
def FitBackgroundModel(success, fail, *args):
    """
    Python wrapper for C++ executable 'FitBackgroundModel'.
    
    Arguments:
    - `success`: bookkeeping file for successful configurations
    - `fail`: bookkeeping file for failed configurations
    - `*args`: arguments for 'FitBackgroundModel'
    """

    cmd = ("FitBackgroundModel",) + args
    msg = "Calling: `" + " ".join(cmd) + "`"
    print "\n"
    print "*"*(len(msg)+6)
    print "*" + " "*(len(msg)+4) + "*"
    print "*  " + msg + "  *"
    print "*" + " "*(len(msg)+4) + "*"
    print "*"*(len(msg)+6)
    print
    sys.stdout.flush()
    start = datetime.datetime.now()
    p = subprocess.Popen(("FitBackgroundModel",) + args)
    while True:
        # wait at most for 20 minutes to let the process finish:
        if p.poll() == None:
            if (datetime.datetime.now() - start).seconds > 1200:
                print "Process killed because it took longer than 20 minutes."
                p.kill()
                with open(fail, "a") as f:
                    fcntl.flock(f, fcntl.LOCK_EX)
                    f.write(args.__repr__()+"\n")
                    fcntl.flock(f, fcntl.LOCK_UN)
                break
            else:               
                time.sleep(10)  # wait for 30 seconds to poll again
        else:
            break
    if p.poll() == 0:
        with open(success, "a") as f:
            fcntl.flock(f, fcntl.LOCK_EX)
            f.write(args.__repr__()+"\n")
            fcntl.flock(f, fcntl.LOCK_UN)
    else:
        with open(fail, "a") as f:
            fcntl.flock(f, fcntl.LOCK_EX)
            f.write(args.__repr__()+"\n")
            fcntl.flock(f, fcntl.LOCK_UN)


################################################################################
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
