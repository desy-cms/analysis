#!/usr/bin/python
import os,sys
from subprocess import call
import logging

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)

def main():
    
    logging.basicConfig(format='%(asctime)s %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
    logging.info('\n Start limits calculation with combine tool. \n')

    #working directory with datacards and stored output:
    reldir = os.environ['CMSSW_BASE'] + '/src/Analysis/MssmHbb/datacards/20161013/unbinned/'
    #array of mass points
#    mass = ['200','250','300','350','400','500','600','700','900','1100','1300']
    mass = ['300','350','400','500','600','700','900','1100','1300','250','200']
    #combine preferences
    combine = 'combine -M Asymptotic --rMin -20 --rMax 20 -n Hbb -m '
    #template of the datacard name:
    data_cards = ['hbb_mbb' + i + '_mssm-13TeV.txt' for i in mass ]
    #card with limits:
    hbblimit = 'Hbb.limits'
    #directory with Combine Tool release (747)
    cmssw747 = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_4_7/src/'
    #temproary dir to stopre and process datacards
    temp_dir = cmssw747 + 'HiggsAnalysis/CombinedLimit/tempr/'

    #Clean temp directory:
    if os.path.exists(temp_dir):
        call('rm -rf ' + temp_dir,shell=True)    
    call('mkdir ' + temp_dir,shell=True)

    #Copy datacards form workdir to CMSSW_7_4_7 tempr folder
    if not os.path.isdir(reldir):
        logging.error('Wrong dir name: ' + reldir + ' was specified')
        return
    for i in data_cards: call('cp ' + reldir + '/' + i + ' ' + temp_dir,shell=True)

    #Need to switch to CMSSW_7_4_7 release, make cmsenv etc
    with cd(cmssw747):
        #make cmsenv
        call('eval `scramv1 runtime -sh`',shell=True)
        with cd(temp_dir):
            #run combine tool:
            for i in mass:
                logging.info(combine + i + ' hbb_mbb' + i + '_mssm-13TeV.txt > log_asymp_m' + i + '.txt')
                call(combine + i + ' hbb_mbb' + i + '_mssm-13TeV.txt > log_asymp_m' + i + '.txt',shell=True)
                call('readlink -f higgsCombineHbb.Asymptotic.mH' + i + '.root >> ' + hbblimit ,shell=True)
            call('cp -r * ' + reldir,shell=True)
    
    logging.info("Limits calculation with combine tool was done")
    #Visualisation of limits:
    with cd(reldir):
        #Run PlotLimit 
        call('PlotLimit -i ' + hbblimit,shell=True)
    logging.info('Done!')

    
    
if __name__ == '__main__':
    main()
       
