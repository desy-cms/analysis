'''Module to merge output from the Bias test

'''

from runBiasTest import GetSubrange,MakeCleanDir
import os
import glob
import subprocess
from shutil import copy, rmtree
#user includes
from bias_config import *
from Bias_output_merger import RemoveSubDirs, MergeFiles
from runFitTest import nfits, njobs, nsigma, fit_name, add_signal, mass, fix_turnon

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

if __name__ == '__main__':
    input = os.environ['CMSSW_BASE'] + '/src/Analysis/BackgroundModel/test/Prescale_v5/' + fit_name + '/workspace/FitContainer_workspace.root'
    #inputs = ['berneff8_200_to_650_10GeV','berneff9_200_to_650_10GeV_G4','extnovoeffprod_200to650_10GeV_G4','superdijeteffprod2_200_to_650_10GeV_G4']
    base_name = 'FitTest_' + str(nfits/njobs) + 'fits_' + str(nsigma) + 'sigma'
    if(add_signal): base_name += '_signal' + mass
    if(fix_turnon): base_name += '_fixedTurnOn'
    base_name += '_' + fit_name
    clean_parts = True
    bias_out = './'
    
    sub_dirs = MergeFiles(base_name,bias_out,'part',False,'Fit')
    if(clean_parts and sub_dirs[0] != ''): RemoveSubDirs(sub_dirs)
    