from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
MASS = '300'
DATASET  = 'SUSYBBHToBB_M-'+MASS+'_13TeV-pythia6-tauola'
DATANAME = 'MCRUN2_74_V7-GEN-SIM-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'PYTHIA6_Tauola_gg_bbH'+MASS+'_bb_13TeV_cfg.py'
config.JobType.pluginName = 'PrivateMC'
config.JobType.outputFiles = ['gensim.root']

config.section_('Data')
config.Data.primaryDataset = DATASET
config.Data.publishDBS = 'phys03'
config.Data.publication = True
config.Data.unitsPerJob = 100
NJOBS = 500  # This is not a configuration parameter, but an auxiliary variable used in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.splitting = 'EventBased'
config.Data.publishDataName = DATANAME

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
