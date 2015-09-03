from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
DATASET  = 'RelValTTbar_13'
DATANAME = 'OpenHLT-742_MCRUN2_74_V9-hlt_run2_mssmhbb_scratch_V47-Producers-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'openhlt_go_prod.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V56_cfg.py', 'setup_740_GRun_V61_cff.py']
config.JobType.outputFiles = ['Producers.root']

config.section_('Data')
config.Data.inputDataset = '/RelValTTbar_13/CMSSW_7_4_1-PU25ns_MCRUN2_74_V9_gensim71X-v1/MINIAODSIM'
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = DATANAME
config.Data.unitsPerJob = 1
#config.Data.totalUnits = 50
config.Data.splitting = 'FileBased'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
