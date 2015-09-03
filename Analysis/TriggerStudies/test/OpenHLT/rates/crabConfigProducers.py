from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
PTHATRANGE = '1800'
DATASET  = 'QCD_Pt_'+PTHATRANGE+'_Fall13dr_TSG'
DATANAME = 'OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
config.JobType.psetName = 'openhlt_go_prod.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['hlt_run2_mssmhbb_scratch_V47_cfg.py', 'setup_740_GRun_V36_cff.py']
config.JobType.outputFiles = ['Producers.root']

config.section_('Data')
INPUT = {}
INPUT['5to10']      = '/QCD_Pt-5to10_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['10to15']     = '/QCD_Pt-10to15_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v3/GEN-SIM-RAW'
INPUT['14to30']     = '/QCD_Pt-15to30_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['30to50']     = '/QCD_Pt-30to50_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['50to80']     = '/QCD_Pt-50to80_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['80to120']    = '/QCD_Pt-80to120_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['120to170']   = '/QCD_Pt-120to170_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['170to300']   = '/QCD_Pt-170to300_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['300to470']   = '/QCD_Pt-300to470_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['470to600']   = '/QCD_Pt-470to600_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['600to800']   = '/QCD_Pt-600to800_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['800to1000']  = '/QCD_Pt-800to1000_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['1000to1400'] = '/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['1400to1800'] = '/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['1800']       = '/QCD_Pt-1800_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v2/GEN-SIM-RAW'
config.Data.inputDataset = INPUT[PTHATRANGE]
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.publishDataName = DATANAME
config.Data.unitsPerJob = 1
config.Data.totalUnits = 50
config.Data.splitting = 'FileBased'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
