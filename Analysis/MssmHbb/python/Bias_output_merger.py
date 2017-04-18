'''Module to merge output from the Bias test

'''

from runBiasTest import GetSubrange,MakeCleanDir
import os
import glob
import subprocess
from shutil import copy, rmtree

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

def RemoveSubDirs(dirs):
    '''Module to remove the directories from the list recursively.
    
    '''
    for d in dirs:
        rmtree(d)
   

def MergeFiles(base_name,input_folder,sub_name='part'):
    '''Function to merge files accordint to the base_name 
    and partial names of sub_files.
    
    '''
    os.chdir(input_folder)
    template_name = sub_name + '*' + base_name
    #list of the root files in the folders
    root_list = glob.glob(template_name + '/' + template_name + '.root')
    #exception if no sub-folders
    if len(root_list) == 0:
        print('No sub-folders for ' + base_name) 
        return ['']
    #clean folder for an output
    if not os.path.exists(base_name): os.makedirs(base_name)
    try:
        root_list_joined = ' '.join(root_list)
        haddd_command = 'hadd -f ' + base_name + '/' + base_name + '.root ' + root_list_joined
        subprocess.check_call(haddd_command,shell=True)
    except subprocess.CalledProcessError:
        print ('Error for hadd -f ' + 'hadd -f ' + base_name)
        return ['']
    
    [copy(f,base_name + '/' + f.split('/')[1]) for f in glob.glob(root_list[0].split('/')[0] +  '/Toy_*')]

    return glob.glob(template_name)
    

if __name__ == '__main__':
    #clean partial dirs
    clean_parts = True
    #list of mass points
    mass = [300,350,400,500,600,700,900,1100,1300]#[300,350,400,500]#[300,350,400,500]#,600,700,900,1100,1300]#
    #signal strength to be uaed
    signal_strengths = [0,2,4]
    #sanity check falg. If yes - alternative = pdf will be used
    sanity_check = True
    #bg only?
    bg_only = False
    #functions to be used
    pdf = { 'sr1' : 'extnovoeffprod_G4_R1', 'sr2' : 'novosibirsk_G4_R2', 'sr3' : 'novosibirsk_G4_R3' }
    alternative = { 'sr1' : 'dijetv3logprod_G4_R1', 'sr2' : 'dijetexp_G4_R2', 'sr3' : 'dijetexp_G4_R3'}
#     alternative = { 'sr1' : 'berneffprod9par_G4_R1', 'sr2' : 'dijetexp_G4_R2', 'sr3' : 'dijetexp_G4_R3'}
    #folder with Bias outputs
    bias_out = './'
    for m in mass:
        sub_range = GetSubrange(m)
        if sanity_check: alternative[sub_range] = pdf[sub_range]
        for mu in signal_strengths:
            base_name = 'BiasTest_toy' + alternative[sub_range] + '_fit' + pdf[sub_range] + '_' + str(m) + 'GeV_r' + str(mu) 
            sub_dirs = MergeFiles(base_name,bias_out)
            if(clean_parts and sub_dirs[0] != ''): RemoveSubDirs(sub_dirs)
    