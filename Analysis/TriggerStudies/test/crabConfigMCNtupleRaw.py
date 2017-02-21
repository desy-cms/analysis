from WMCore.Configuration import Configuration

config = Configuration()

# -- General
config.section_('General')
config.General.transferOutputs = True
config.General.requestName     = ''
config.General.workArea        = 'crab_projects_QCD_pythia8'
config.General.transferLogs    = True
#config.General.requestName     = 'QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8'

# -- JobType
config.section_('JobType')
config.JobType.psetName    = 'QCD_Pythia8_GRun-L1Trepack_HLTUserMC-RAW-HLT2_Ntuple.py'
config.JobType.pluginName  = 'Analysis'
config.JobType.outputFiles = ['ntuple.root']
config.JobType.numCores    = 8

# -- Data
config.section_('Data')
config.Data.publication      = False
#config.Data.publishDBS       = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.unitsPerJob      = 20
config.Data.totalUnits       = -1
config.Data.splitting        = 'FileBased'
config.Data.outLFNDirBase    = '/store/user/rwalsh/Analysis/Ntuples/TriggerStudies/MC/RunIISummer16DR80-FlatPU28to62/80x_qcd_pythia8_usermcv29_raw_v1/'
config.Data.outputDatasetTag = 'crab_QCD_Pythia8'

#config.Data.inputDataset          = '/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/GEN-SIM-RAW'

# extended samples
#config.Data.inputDataset          = '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/GEN-SIM-RAW'
#config.Data.inputDataset          = '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/GEN-SIM-RAW'
config.Data.inputDataset          = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16DR80-FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/GEN-SIM-RAW'


#config.General.requestName     = 'QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8'
#config.General.requestName     = 'QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8'

# extended samples
#config.General.requestName     = 'QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8_ext'
#config.General.requestName     = 'QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8_ext'
config.General.requestName     = 'QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_ext'


# -- User
config.section_('User')

# -- Site
config.section_('Site')
config.Site.storageSite = 'T2_DE_DESY'
