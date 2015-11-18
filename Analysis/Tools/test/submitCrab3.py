#!/usr/bin/env python

import os.path
import urllib2

from WMCore.Configuration import Configuration

# ---
# Some parameter steering
PROCESS         = 'MSSMHbb'
UNITS_PER_JOB   = 1
TYPE            = 'MC'
PSET            = 'ntuplizer_mc.py'
CAMPAIGN        = 'Fall15.SomeVersion'
BASEOUTDIR      = '/store/user/MyUserName/Analysis/Ntuples/' + PROCESS
URL             = 'http://www.desy.de/~MyUserName/cms/analysis/samples/miniaod'

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
      config.JobType.psetName = 'ntuplizer_mc.py'
   if TYPE == 'DATA':
      config.Data.splitting   = 'LumiBased'
      config.JobType.psetName = 'ntuplizer_data.py'
      
   config.General.workArea += '_' + PROCESS
   config.Data.unitsPerJob = UNITS_PER_JOB
   
   for dataset in datasets:
      dataset = dataset.split('\n')[0]
      dataset_name = dataset.split('/')[1]
      dataset_cond = dataset.split('/')[2]
      dataset_tier = dataset.split('/')[3]
      config.Data.inputDataset    = dataset
      config.Data.publishDataName = dataset_cond
      config.General.requestName  = dataset_name
      config.Data.outLFNDirBase   = BASEOUTDIR + '/' + dataset_tier + '/' + CAMPAIGN + '/'
      config.JobType.psetName    = PSET
      crabCommand('submit', config = config)

# _________________________________________________________________________
