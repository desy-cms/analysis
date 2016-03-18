#!/usr/bin/env python

import os
import os.path as op
import sys
import argparse
import subprocess


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
    parser.add_argument("-o", "--output-dir", dest = "output_dir",
                        metavar = "PATH", required = True,
                        help = "Path where output is stored.")
    args = parser.parse_args(argv)

    job_configs = create_job_configs(args.output_dir)
    split_configs = split_batch_jobs(job_configs, 9)
    submit_batch_jobs(split_configs, args.output_dir)

    
################################################################################
def submit_batch_jobs(splitted_configs, out_dir):
    """
    Submit batch jobs to the NAF.
    
    Arguments:
    - `splitted_configs`: list of configuration lists
    """

    submit_dir = op.join(out_dir, "submit")
    
    try:
        os.makedirs(submit_dir)
    except OSError as e:
        if e.args == (17, "File exists"):
            pass
        else:
            raise

    i = 0
    for configs in splitted_configs:
        script = script_template.format(arguments = configs.__repr__(),
                                        success = op.join(out_dir, "success.txt"),
                                        fail = op.join(out_dir, "fail.txt"))
        file_name = op.join(submit_dir,
                            "submit_background_model_studies_"+
                            str(i).zfill(4)+".sh")
        with open(file_name, "w") as f:
            f.write(script)
        subprocess.call(["qsub", file_name], cwd = submit_dir)
        i += 1


def split_batch_jobs(configs, size):
    """
    Splits into lists of `size` job configurations.
    
    Arguments:
    - `configs`: unsplitted job configuration list
    - `size`: size of the created batch job configuration lists
    """
    
    for i in xrange(0, len(configs), size):
        yield configs[i:i+size]


def create_job_configs(out_dir):
    """
    Creates job configurations.

    Arguments:
    - `out_dir`: path to the output location
    """

    data_dir = op.join(os.environ["CMSSW_BASE"], "src", "Analysis",
                       "BackgroundModel", "data")

    inputs = {"8TeV":  ["--input_file",
                        op.join(data_dir, "HIG14017_HighMass2012_Packed_M350_inputs.root")],
              "13TeV": ["--input_tree_file",
                        op.join(data_dir, "DoubleBTagSelection_13TeV.root")]}
    models = {"polynomials": ["bernstein", "berneffprod"], #"Chebychev"
              "non-polynomials": ["bukin", "crystalball", "novosibirsk"]}
    fit_min = [str(x) for x in xrange(0, 301, 10)]
    fit_max = [str(x) for x in xrange(600, 2001, 50)]
    orders = [str(x) for x in xrange(3, 20)]

    configs = []
    for inp in inputs:
        for model_type in models:
            for model in models[model_type]:
                for mbb_min in fit_min:
                    for mbb_max in fit_max:
                        if model_type == "polynomials":
                            for order in orders:
                                config = ["-b", model+","+order,
                                          "-o", op.join(out_dir, inp, "_".join([model, order, "min"+mbb_min, "max"+mbb_max])),
                                          # "-p",
                                          "--fit_min", mbb_min,
                                          "--fit_max", mbb_max]
                                config.extend(inputs[inp])
                                configs.append(config)
                        else:
                            config = ["-b", model,
                                      "-o", op.join(out_dir, inp, "_".join([model, "min"+mbb_min, "max"+mbb_max])),
                                      # "-p",
                                      "--fit_min", mbb_min,
                                      "--fit_max", mbb_max]
                            config.extend(inputs[inp])
                            configs.append(config)

    return configs


################################################################################
script_template="""\
#!/bin/zsh
#$ -S /bin/zsh
#$ -l h_rt=02:59:59
#$ -j y
#$ -cwd
#$ -l site=hh
#$ -V
#$ -M {user:s}@mail.desy.de
#$ -m ae

echo "Job started at: $(date)"
fit_background_model.py --success {{success:s}} --fail {{fail:s}} --list "{{arguments:s}}"
echo "Job ended at: $(date)"
""".format(user = os.environ["USER"])


################################################################################
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
