#!/usr/bin/env python

import os
import os.path as op
import sys
import argparse
import subprocess
import ROOT
import submit_background_model_studies as sbms


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
        description = "Analyze background model studies.")
    parser.add_argument("-i", "--input-dir", dest = "input_dir",
                        metavar = "PATH", required = True,
                        help = "Path where input is stored.")
    args = parser.parse_args(argv)

    job_names = [c[c.index("-o")+1]
                 for c in sbms.create_job_configs(args.input_dir)]

    results = map(lambda x: (x.split(args.input_dir)[1], get_fit_result(x)),
                  job_names)
    results.sort(key = lambda x: x[1])
    for r in results:
        print r


################################################################################
def get_fit_result(input_dir):
    """
    Returns normalized chi^2 of the background-only fit for a given fit
    configuration.

    Arguments:
    - `input_dir`: directory where fit result is stored
    """

    file_name = op.join(input_dir, "workspace", "FitContainer_workspace.root")
    fit_file = ROOT.TFile(file_name, "read")
    tree = fit_file.Get("fit_b")
    try:
        tree.GetEntry(0)
    except AttributeError as e:
        if e.args[0] == "'TObject' object has no attribute 'GetEntry'":
            return -10000.0
        else:
            raise
    result = tree.normChi2
    fit_file.Close()
    return result


################################################################################
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
