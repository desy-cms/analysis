#!/usr/bin/env python
"""Script to copy signal root files.

"""
import shutil
import os

__author__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

def Message(in_file, out_file):
    """ Function to print message about copying
    
    """
    print "COPY ROOT FILE: " + in_file + "\nTO: " + out_file + " .\n"

if __name__ == '__main__':
    CMSSW_BASE = os.environ['CMSSW_BASE']
    input_dir = '/src/Analysis/MssmHbb/output/'
    output_dir = input_dir
    
    input_selection = 'ReReco_PU_test_subrange_presc'
    output_name = 'ReReco_newBGTemplates_20170126'
    
    masses = [300,350,400,500,600,700,900,1100,1300]
    
    for m in masses:
        in_name = CMSSW_BASE + input_dir + 'MssmHbbSignal_' + input_selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-' + str(m) + '_TuneCUETP8M1_13TeV-pythia8.root'
        out_name = CMSSW_BASE + output_dir + 'MssmHbbSignal_' + output_name + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-' + str(m) + '_TuneCUETP8M1_13TeV-pythia8.root'
        if not os.path.exists(in_name):
            raise IOError("No root file at " + in_name)
        shutil.copyfile(in_name,out_name)
        Message(in_name,out_name)
        