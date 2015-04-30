from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
DATASET  = 'MinBias_TuneZ2star_900GeV_pythia6'
DATANAME = 'MCRUN2_74_V7-GEN-SIM-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'MinBias_TuneZ2star_900GeV_pythia6_GENSIM_cfg.py'
config.JobType.pluginName = 'PrivateMC'
config.JobType.outputFiles = ['gensim.root']

config.section_('Data')
config.Data.primaryDataset = DATASET
config.Data.publishDBS = 'phys03'
config.Data.publication = True
config.Data.unitsPerJob = 5000
NJOBS = 2000  # This is not a configuration parameter, but an auxiliary variable used in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.splitting = 'EventBased'
config.Data.publishDataName = DATANAME

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
