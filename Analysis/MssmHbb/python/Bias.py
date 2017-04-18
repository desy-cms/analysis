#!/usr/bin/python

'''Macro to get the bias to be encoded in the datacards.

'''

from pydoc import __author__

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

def PrintInformation(r,dr_up,dr_dn,mass,bias):
    '''Function to print important for bias information.
    
    '''
    print ('M = ' + mass + ', r = ' + r + ' ' + dr_up + '/' + dr_dn + ', bias = ' + str(bias) + ', beta_data_card = ' + str(bias * (float(dr_up) - float(dr_dn))/2.))

def getLineWithSignalStrength(log_file):
    '''Function to get line from log file.
    
    Function to get line with signal strength and its errors
    '''
    #define keyword to look at:
    keyword = 'Best fit r:'
    #read file line-by-line FROM THE END of the file
    for line in reversed(open(log_file,mode='r').readlines()):
        #check whether keyword is in this line
        if keyword in line:
            return line
    #if no such line
    raise AttributeError("ERROR: No " + keyword + " in log file " + log_file + " . Check this log file!!")

def InformationAboutBias(log_file,mass,bias):
    '''Function to print information about the bias.
    
    '''
    line_with_r = getLineWithSignalStrength(log_file)
    r = line_with_r.split()[3]
    dr_up = line_with_r.split()[4].split('/',1)[1]
    dr_dn = line_with_r.split()[4].split('/',1)[0]
    PrintInformation(r,dr_up,dr_dn,mass,bias)

if __name__ == '__main__':
    #bias from @chayanit for each mass point
    bias = {'300': 1.,
            '350' : 1.,
            '400' : 1.,
            '500' : 1.,
            '600' : 0.28,
            '700' : -0.07,
            '900' : -0.14,
            '1100': -0.59,
            '1300': -0.19}
    #folder with mll fits
    input_folder = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201704/04/mll_forBias/'
    #log filies with output of MLL
    log_files = [input_folder + 'mll_M-' + m + '/log_' + m + '.o' for m in bias]
    #loop over the mass points
    for mass in bias:
        #log filie with output of MLL
        log_file = input_folder + 'mll_M-' + mass + '/log_' + mass + '.o'
        InformationAboutBias(log_file,mass,bias[mass])
