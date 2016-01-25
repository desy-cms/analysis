#!/usr/bin/env python

import os.path
import urllib2

from WMCore.Configuration import Configuration

# ---
# Some parameter steering
PROCESS         = 'BTagCSVData'
TYPE            = 'DATA'
if TYPE == 'DATA':
	UNITS_PER_JOB   = 1000
if TYPE == 'MC':
	UNITS_PER_JOB   = 10
PSET            = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/Ntuplizer/test/btag_data.py'
CAMPAIGN        = 'Fall15.20160121'
BASEOUTDIR      = '/store/user/rshevche/Analysis/Ntuples/' + PROCESS
URL             = '/nfs/dust/cms/user/shevchen/samples/miniaod'
#URL             = 'http://www.desy.de/~walsh/cms/analysis/samples/miniaod'

# ---
dataset_list    = URL + '/' + PROCESS + '.txt'
#datasets        = urllib2.urlopen(dataset_list)
datasets        = open(dataset_list,'r')

# _________________________________________________________________________

if __name__ == '__main__':

   from CRABAPI.RawCommand import crabCommand
   from CRABClient.ClientExceptions import ClientException
   from httplib import HTTPException
    
   from Analysis.Tools.crabConfig import crabConfig
   config = crabConfig()
   
   if TYPE == 'MC':
      config.Data.splitting   = 'FileBased'
      config.JobType.psetName = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/Ntuplizer/test/qcd_mc.py'
   if TYPE == 'DATA':
      config.Data.splitting   = 'LumiBased'
      config.JobType.psetName = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/Ntuplizer/test/btag_data.py'
      
   config.General.workArea += '_' + PROCESS
   config.Data.unitsPerJob = UNITS_PER_JOB
   
   for dataset in datasets:
      dataset = dataset.split('\n')[0]
      dataset_name = dataset.split('/')[1]
      dataset_cond = dataset.split('/')[2]
      dataset_tier = dataset.split('/')[3]
      print dataset_name, dataset_cond, dataset_tier
      config.Data.inputDataset    = dataset
      config.Data.outputDatasetTag = dataset_cond
      config.General.requestName  = dataset_name+ '_' + dataset_cond
      config.Data.outLFNDirBase   = BASEOUTDIR + '/' + dataset_tier + '/' + CAMPAIGN + '/'
      config.JobType.psetName    = PSET
      crabCommand('submit', config = config)

# _________________________________________________________________________
