#!/usr/bin/python
'''Macro to run BG fit on prescaled data

'''

import os
from subprocess import Popen,call

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

if __name__ == '__main__':
    #deffine prescales to be used
    prescales = ['G' + str(i) for i in range(1,10)]#['']#
    #define functions to be fitted
    pdfs = ['superdijeteffprod,2']#['extnovoeffprod','superdijeteffprod,2','berneffprod,8']
    # folder with a data
    data_folder = os.environ['CMSSW_BASE'] + '/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/'
    #data name template
    data_name = 'TripleBTagReverseSelectionBtoH2016_prescale_13TeV_'#'TripleBTagReverseSelectionBtoH2016_13TeV'#
    #output folder
    out_folder = os.environ['CMSSW_BASE'] + '/src/Analysis/BackgroundModel/test/'
    #output fit folder
    out_fit_folder = 'Prescale_' #'fullBBnB'#
    if not os.path.exists(out_folder): os.mkdir(out_folder)
    #fit parameters:
    fit_min = 200
    fit_max = 650
    nbins = 45
    bin_size = (fit_max - fit_min)/nbins
    
    #run the fits:
    for prescale in prescales:
        for pdf in pdfs:
            command = 'FitBackgroundModel -t ' + data_folder + data_name + prescale + '.root'
            #add Output folder
            function_folder = pdf + '_' +str(fit_min) + 'to' + str(fit_max) + '_' + str(bin_size) + 'GeV_' + prescale + '_' + str(nbins) + 'bins'
            command += ' -o ' + out_folder + out_fit_folder + prescale + '/' + function_folder 
            #add fit parameters
            command += ' --fit_min ' + str(fit_min) + ' --fit_max ' + str(fit_max) + ' --nbins ' + str(nbins)
            command += ' -b ' + pdf 
            #verbosity
            command += ' -v 5'
            #log 
            command += ' > ' + out_folder + function_folder + '.o'
            #run
            #print command
            proc = Popen(command,shell = True)