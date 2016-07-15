#!/usr/bin/env python

import os
import os.path
import urllib2
import importlib

from WMCore.Configuration import Configuration


# ---
# Some parameter steering
PROCESS         = 'QCD_pythia8'
UNITS_PER_JOB   = 5
TYPE            = 'MC'
PSET            = 'ntuplizer_mc_765_summer_conferences_2016_v1.py'
CAMPAIGN        = 'Fall15/76x_summer_conferences_2016_v1'
BASEOUTDIR      = '/store/user/rwalsh/Analysis/Ntuples/' + TYPE + '/' + CAMPAIGN
URL             = 'http://www.desy.de/~walsh/cms/analysis/samples/miniaodsim/Fall15'

# ---
dataset_list    = URL + '/' + PROCESS + '.txt'
datasets        = urllib2.urlopen(dataset_list)

import FWCore.ParameterSet.Config as cms
#from ntuplizer_mc_765_summer_conferences_2016_v1 import process
pset = PSET.split('.')[0]
process = __import__(pset).process #(see why it does not work!)

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
   config.Data.splitting   = 'FileBased'
   config.Data.unitsPerJob  = UNITS_PER_JOB
   config.Data.totalUnits   = -1
   config.Data.outLFNDirBase   = BASEOUTDIR + '/'
#   config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader/'
#   config.Data.allowNonValidInputDataset = True    # If dataset not valid yet, will run over valid files only

# ====== JOBTYPE
   config.JobType.inputFiles = ['Fall15_25nsV2_MC_PtResolution_AK4PFPuppi.txt','Fall15_25nsV2_MC_PtResolution_AK4PFchs.txt','Fall15_25nsV2_MC_SF_AK4PFPuppi.txt','Fall15_25nsV2_MC_SF_AK4PFchs.txt']

   for dataset in datasets:
      dataset=dataset.replace(" ", "")
      cross_section = 1.
      if len(dataset.split(',')) > 1:
         cross_section = dataset.split(',')[1].split('\n')[0]
         dataset = dataset.split(',')[0]
      else:
         dataset = dataset.split('\n')[0]
      dataset_name = dataset.split('/')[1]
      dataset_cond = dataset.split('/')[2]
      dataset_tier = dataset.split('/')[3]
#      
      config.Data.inputDataset     = dataset
      config.Data.outputDatasetTag = dataset_cond
#      
      config.General.requestName  = dataset_name
# use if needed in private productions (modify accordingly)
#      processname = dataset_cond.split('_')
#      config.General.requestName  += '_'+processname[0]+'-'+processname[1]+'_oldGT'
      print config.General.requestName 
#     
      process.MssmHbb.CrossSection = cms.double(cross_section)
      psettmp = pset+'_tmp.py'
      f = open(psettmp, 'w')
      f.write(process.dumpPython())
      f.close()
#      
      config.JobType.psetName    = psettmp
#
      crabCommand('submit', config = config)
#
      os.remove(psettmp)

# _________________________________________________________________________
