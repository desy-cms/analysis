#!/usr/bin/python

'''Macro to combine datacards from 7 + 8 + 13 TeV.

'''

from subprocess import Popen
from pydoc import __author__
import os

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

if __name__ == '__main__':
    
    #input dir with datacards
    input_dir = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201703/08/asymptotic/combination/mssm/'
    os.chdir(input_dir)
    #mass points
    masses = [300,350,400,500,600,700,900,1100,1300]
    for m in masses:
        card_7p8_TeV = 'Comb_7TeV_HIG14017_HighMass2012_Packed_M' + str(m) + '_card_theoryUnc.txt'
        card_8TeV    = 'HIG14017_HighMass2012_Packed_M' + str(m) + '_card_theoryUnc.txt'
        card_13TeV   = 'hbb_mbb' + str(m) + '_mssm-13TeV.txt'
        card_comb    = 'Comb_7TeV_8TeV_HIG16018_13TeV_Packed_M' + str(m) + '_card.txt'
        if m < 600:
            comb_command = 'combineCards.py ' + card_7p8_TeV + ' ' + card_13TeV + ' > ' + card_comb
        elif m < 1100:
            comb_command = 'combineCards.py ' + card_8TeV + ' ' + card_13TeV  + ' > ' + card_comb
        else:
            comb_command = 'combineCards.py ' + card_13TeV  + ' > ' + card_comb
        #run combination script
        Popen(comb_command,shell=True)
        