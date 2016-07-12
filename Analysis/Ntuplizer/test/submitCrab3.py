#!/usr/bin/env python

import os.path
import urllib2

from WMCore.Configuration import Configuration

# ---
# Some parameter steering
PROCESS         = 'QCD_pythia8_1'
UNITS_PER_JOB   = 2
TYPE            = 'MC'
PSET            = 'ntuplizer_mc_76x_summer_conferences_2016_v1.py'
CAMPAIGN        = 'Fall15/76x_summer_conferences_2016_v1'
BASEOUTDIR      = '/store/user/rwalsh/Analysis/Ntuples/' + TYPE + '/' + CAMPAIGN
URL             = 'http://www.desy.de/~walsh/cms/analysis/samples/miniaodsim/Fall15'

# ---
dataset_list    = URL + '/' + PROCESS + '.txt'
datasets        = urllib2.urlopen(dataset_list)

# _________________________________________________________________________

if __name__ == '__main__':

   from CRABAPI.RawCommand import crabCommand
   from CRABClient.ClientExceptions import ClientException
   from httplib import HTTPException
    
   from Analysis.Tools.crabConfig import crabConfig
   config = crabConfig()
   
   if TYPE == 'MC':
      config.Data.splitting   = 'FileBased'
#      config.JobType.psetName = 'ntuplizer_mc.py'
   if TYPE == 'DATA':
      config.Data.splitting   = 'LumiBased'
#      config.JobType.psetName = 'ntuplizer_data.py'
      
#   config.General.workArea += '_' + PROCESS

# If dataset not valid yet, will run over valid files only
#   config.Data.allowNonValidInputDataset = True
   
   for dataset in datasets:
      dataset = dataset.split('\n')[0]
      dataset_name = dataset.split('/')[1]
      dataset_cond = dataset.split('/')[2]
      dataset_tier = dataset.split('/')[3]
#      
      config.Data.inputDataset    = dataset
#      config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader/'
      config.Data.outputDatasetTag = dataset_cond
      config.Data.unitsPerJob  = UNITS_PER_JOB
      config.Data.totalUnits   = -1
      config.Data.outLFNDirBase   = BASEOUTDIR + '/'
#      
# use if needed in private productions (modify accordingly)
      config.General.requestName  = dataset_name
# use if needed in private productions (modify accordingly)
#      processname = dataset_cond.split('_')
#      config.General.requestName  += '_'+processname[0]+'-'+processname[1]+'_oldGT'
#      
      config.JobType.psetName    = PSET
      config.JobType.inputFiles = ['Fall15_25nsV2_MC_PtResolution_AK4PFPuppi.txt','Fall15_25nsV2_MC_PtResolution_AK4PFchs.txt','Fall15_25nsV2_MC_SF_AK4PFPuppi.txt','Fall15_25nsV2_MC_SF_AK4PFchs.txt']
      crabCommand('submit', config = config)

# _________________________________________________________________________
