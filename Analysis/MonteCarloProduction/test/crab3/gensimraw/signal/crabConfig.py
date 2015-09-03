from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
MASS = '1100'
TYPE  = 'MSSMHBB_M-'+MASS
DATANAME = 'RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-GEN_SIM_RAW-v1'
config.General.requestName = TYPE+'-'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'SUSYGluGluToBBHToBB_M-'+MASS+'_TuneCUETP8M1_13TeV-pythia8-GEN_SIM_RAW_cfg.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['step1.root']

config.section_('Data')
config.Data.inputDataset = '/SUSYGluGluToBBHToBB_M-'+MASS+'_TuneCUETP8M1_13TeV-pythia8/RunIIWinter15GS-MCRUN2_71_V1-v1/GEN-SIM'
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 1
NJOBS = 100  # This is not a configuration parameter, but an auxiliary variable used in the next line.
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.publishDataName = DATANAME
#config.Data.outLFN = '/store/user/rwalsh/tmp'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
config.Site.whitelist = ['T2_DE_DESY']
config.Site.storageSite = 'T2_DE_DESY'
