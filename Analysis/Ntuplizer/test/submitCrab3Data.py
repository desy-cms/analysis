#!/usr/bin/env python

import os.path
import urllib2

from WMCore.Configuration import Configuration

# ---
# Some parameter steering
PROCESS         = '16Dec2015'
UNITS_PER_JOB   = 1000
TYPE            = 'DATA'
PSET            = 'ntuplizer_data_765_summer_conferences_2016_v1.py'
CAMPAIGN        = 'Run2015/76x_summer_conferences_2016_v1'
BASEOUTDIR      = '/store/user/rwalsh/Analysis/Ntuples/' + TYPE + '/' + CAMPAIGN
URL             = 'http://www.desy.de/~walsh/cms/analysis/samples/miniaod/Run2015'

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

# ====== GENERAL
#   config.General.workArea += '_' + PROCESS


# ====== DATA   
   config.Data.splitting   = 'LumiBased'
   config.Data.unitsPerJob  = UNITS_PER_JOB
   config.Data.totalUnits   = -1
   config.Data.outLFNDirBase   = BASEOUTDIR + '/'
#   config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt'
#   config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader/'
#   config.Data.allowNonValidInputDataset = True    # If dataset not valid yet, will run over valid files only

# ====== JOBTYPE
   config.JobType.psetName    = PSET
   config.JobType.inputFiles = ['Fall15_25nsV2_DATA_PtResolution_AK4PFPuppi.txt','Fall15_25nsV2_DATA_PtResolution_AK4PFchs.txt','Fall15_25nsV2_DATA_SF_AK4PFPuppi.txt','Fall15_25nsV2_DATA_SF_AK4PFchs.txt']

   
   for dataset in datasets:
      dataset = dataset.split('\n')[0]
      dataset_name = dataset.split('/')[1]
      dataset_cond = dataset.split('/')[2]
      dataset_tier = dataset.split('/')[3]
#      
      config.Data.inputDataset    = dataset
      config.Data.outputDatasetTag = dataset_cond
#      
      config.General.requestName  = dataset_name
      config.General.requestName += '_'+dataset_cond
      
      print config.General.requestName 
#      
      crabCommand('submit', config = config)

# _________________________________________________________________________
