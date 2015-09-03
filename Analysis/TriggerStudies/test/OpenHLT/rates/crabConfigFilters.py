from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
PTHATRANGE = '1800'
DATASET  = 'QCD_Pt_'+PTHATRANGE+'_Fall13dr_TSG'
DATANAME = 'OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-Filters-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'openhlt_go_filt.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V47_OnlineBTag_cfg.py', 'setup_740_GRun_V36_cff.py']
config.JobType.outputFiles = ['Filters.root']

config.section_('Data')
INPUT = {}
INPUT['5to10']      = ''
INPUT['10to15']     = ''
INPUT['14to30']     = ''
INPUT['30to50']     = ''
INPUT['50to80']     = '/QCD_Pt-50to80_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['80to120']    = '/QCD_Pt-80to120_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['120to170']   = '/QCD_Pt-120to170_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['170to300']   = '/QCD_Pt-170to300_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['300to470']   = '/QCD_Pt-300to470_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['470to600']   = '/QCD_Pt-470to600_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['600to800']   = '/QCD_Pt-600to800_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['800to1000']  = '/QCD_Pt-800to1000_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['1000to1400'] = '/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['1400to1800'] = '/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
INPUT['1800']       = '/QCD_Pt-1800_Tune4C_13TeV_pythia8/rwalsh-OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1-dcc3d7f9b2103fc5465c7cc1bc6f6474/USER'
config.Data.inputDataset = INPUT[PTHATRANGE]
config.Data.inputDBS = 'phys03'
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = DATANAME
config.Data.unitsPerJob = 1
config.Data.splitting = 'FileBased'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
