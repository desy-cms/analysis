from WMCore.Configuration import Configuration

config = Configuration()

# -- General
config.section_('General')
config.General.transferOutputs = True
config.General.requestName     = ''
config.General.workArea        = 'crab_projects_ParkingZeroBias_v1'
config.General.transferLogs    = True
#config.General.requestName     = 'ParkingZeroBias0'

# -- JobType
config.section_('JobType')
config.JobType.psetName    = 'ZeroBias_Run2016H_GRun-L1Trepack_HLTUser-RAW-HLT2_Ntuple.py'
config.JobType.pluginName  = 'Analysis'
config.JobType.outputFiles = ['ntuple.root']
config.JobType.numCores    = 8

# -- Data
config.section_('Data')
config.Data.publication      = False
#config.Data.publishDBS       = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.unitsPerJob      = 5
config.Data.totalUnits       = -1
config.Data.splitting        = 'LumiBased'
config.Data.outLFNDirBase    = '/store/user/rwalsh/Analysis/Ntuples/TriggerStudies/DATA/Run2016/80x_parkingzerobias_run2016hv1_userv29_raw_v1/'
config.Data.lumiMask         = 'Cert_ParkZeroBias_TriggerStudies_JSON.txt'
config.Data.outputDatasetTag = 'crab_ParkingZeroBias_2016Hv1_RAW'

config.Data.inputDataset          = '/ParkingZeroBias1/Run2016H-v1/RAW'
config.General.requestName        = 'ParkingZeroBias1'

# -- User
config.section_('User')

# -- Site
config.section_('Site')
config.Site.storageSite = 'T2_DE_DESY'

