from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
DATASET  = 'SUSYBBHToBB_M-300_13TeV'
DATANAME = 'OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Filt-v2'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
#config.JobType.psetName = 'openhlt_go_prod.py'
config.JobType.psetName = 'openhlt_go_filt.py'
config.JobType.pluginName = 'Analysis'
#config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V47_cfg.py', 'setup_740_GRun_V36_cff.py']
config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V47_OnlineBTag_cfg.py', 'setup_740_GRun_V36_cff.py']
#config.JobType.outputFiles = ['Producers.root']
config.JobType.outputFiles = ['Filters.root']

config.section_('Data')
#config.Data.inputDataset = '/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/yechen-SUSYBBHToBB_M-300_13TeV-pythia6-tauola-RAWSIM-ave_40_bx_25ns-d065fd20afeab0487c05d89037cc39c1/USER'
config.Data.inputDataset = '/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/rwalsh-OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v1-4a506ec4fefd912077c1330a00458e1d/USER'
config.Data.inputDBS = 'phys03'
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = DATANAME
#config.Data.unitsPerJob = 4
config.Data.unitsPerJob = 1
#config.Data.totalUnits = 250
config.Data.splitting = 'FileBased'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
