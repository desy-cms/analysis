#!/usr/bin/python
'''Macro to run Fit-test 

'''

import os,sys
from runBiasTest import MakeCleanDir, submit, CreateCshFile, UpdateSubmissionCsh

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

#NFITS 
nfits = 1000
#nfit per job
njobs = 500
#nsigmas deviation from the mean for gaus randomisation
nsigma = 10
#fit namee
fit_name = 'berneff8_200_to_650_10GeV'
#add signal
add_signal = True
#mass of the signal
mass = '300'
#fix turn-on
fix_turnon = True

def PrepareSubmissionCommand(macro,nfits,nsigma,fix_turnon,input,out_name,parallel_job,parallel = True):
    """Function to prepare command for submission
    
    """
    command = macro + ' -n ' + str(nfits) + ' -i ' + str(input) + ' -s ' + str(nsigma) + ' --plots'
    if(add_signal): 
        command += ' --add_signal --mass ' + mass
    if(fix_turnon): command += ' --fix_turnon '
    command +=  ' -o ' + str(out_name) 
    if(parallel): 
        command += '_part' + str(parallel_job)
    return command

if __name__ == '__main__':
    #input path
    input = os.environ['CMSSW_BASE'] + '/src/Analysis/BackgroundModel/test/Prescale_v5/' + fit_name + '/workspace/FitContainer_workspace.root'
    #inputs = ['berneff8_200_to_650_10GeV','berneff9_200_to_650_10GeV_G4','extnovoeffprod_200to650_10GeV_G4','superdijeteffprod2_200_to_650_10GeV_G4']
    #log file path:
    log_file_path = '/nfs/dust/cms/user/shevchen/output/fittest/'
    #macro to run
    macro = 'FitBG_study'
    out_name=fit_name
    
    dir = MakeCleanDir("submit_fittest")
    for job in range(0,njobs):
        nfits_pjob = nfits / njobs
        command  = PrepareSubmissionCommand(macro,nfits_pjob,nsigma,fix_turnon,input,out_name,job)
        cshFile_name = macro + '_' + str(nfits_pjob) + 'fits_' + str(nsigma) + 'sigma'
        if(add_signal): cshFile_name += '_signal' + mass
        if(fix_turnon): cshFile_name += '_fixedTurnOn'
        cshFile_name += '_' + str(job)
        cshFile = CreateCshFile(cshFile_name,dir)
        UpdateSubmissionCsh(command,cshFile)
        submit(cshFile,'naf',log_file_path)
