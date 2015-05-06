from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
PROCESS = "QCD_Pt-"   # QCD_Pt- or MSSMHbb_
BIN = ""
DATASET  = PROCESS+BIN
OUTDIR = 'ak4GenJets'
VERSION = 'v2'
config.General.requestName = DATASET+'-'+VERSION

config.section_('JobType')
config.JobType.psetName = 'ntuplizer_gen.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['ntuple_ak4genjets.root']

config.section_('Data')
INPUT = {}
INPUT['5to10']      = '/QCD_Pt-5to10_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
INPUT['10to15']     = '/QCD_Pt-10to15_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v3/GEN-SIM-RAW'
INPUT['15to30']     = '/QCD_Pt-15to30_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW'
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
INPUT['M-300']      = '/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/RunIIWinter15GS-MCRUN2_71_V1-v1/GEN-SIM'
INPUT['M-600']      = '/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/RunIIWinter15GS-MCRUN2_71_V1-v1/GEN-SIM'
INPUT['M-1100']     = '/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/RunIIWinter15GS-MCRUN2_71_V1-v1/GEN-SIM'
config.Data.inputDataset = INPUT[BIN]
config.Data.publication = False
config.Data.unitsPerJob = 90
config.Data.splitting = 'FileBased'
config.Data.outLFN = '/store/user/rwalsh/Analysis/MSSMHbbRunII/GEN/'+OUTDIR

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
#config.Site.whitelist = ['T2_DE_DESY', 'T2_DE_RWTH']
config.Site.storageSite = 'T2_DE_DESY'
