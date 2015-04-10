from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
DATASET  = 'SUSYBBHToBB_M-300_13TeV-pythia6-tauola'
DATANAME = 'OpenHLT-740-hlt_run2_mssmhbb_scratch_V23-Producers-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'openhlt_go_prod.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V23_cfg.py', ' setup_740_GRun_V34_cff.py']
config.JobType.outputFiles = ['Producers.root']

config.section_('Data')
config.Data.inputDataset = '/'+DATASET+'/yechen-SUSYBBHToBB_M-300_13TeV-pythia6-tauola-RAWSIM-ave_40_bx_25ns-d065fd20afeab0487c05d89037cc39c1/USER'
config.Data.publishDBS = 'phys03'
config.Data.publication = True
config.Data.unitsPerJob = 200
config.Data.totalUnits = 100000
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'EventBased'
config.Data.publishDataName = DATANAME

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
