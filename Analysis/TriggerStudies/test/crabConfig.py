from WMCore.Configuration import Configuration

config = Configuration()

# -- General
config.section_('General')
config.General.transferOutputs = True
config.General.requestName     = ''
config.General.workArea        = 'crab_projects'
config.General.transferLogs    = True
config.General.requestName     = 'ZeroBias'

# -- JobType
config.section_('JobType')
config.JobType.psetName    = 'ZeroBias_Run2016H_GRun-L1Trepack_HLTUser-RAW-HLT2.py'
config.JobType.pluginName  = 'Analysis'
config.JobType.outputFiles = ['step2_L1REPACK_HLT.root']
config.JobType.numCores    = 4

# -- Data
config.section_('Data')
config.Data.publication      = True
config.Data.publishDBS       = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.unitsPerJob      = 5
config.Data.totalUnits       = -1
config.Data.splitting        = 'LumiBased'
config.Data.outLFNDirBase    = '/store/user/rwalsh/'
config.Data.lumiMask         = 'Cert_RunForTriggerStudies_JSON.txt'
config.Data.outputDatasetTag = 'crab_ZeroBias_2016Hv1'
# datasets
config.Data.inputDataset           = '/ZeroBias/Run2016H-PromptReco-v2/MINIAOD'
config.Data.secondaryInputDataset  = '/ZeroBias/Run2016H-v1/RAW'

# -- User
config.section_('User')

# -- Site
config.section_('Site')
config.Site.storageSite = 'T2_DE_DESY'

