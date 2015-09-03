from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
PTHATRANGE = '50to80'
DATASET  = 'QCD_Pt_'+PTHATRANGE+'_bbEnriched_TuneZ2star_13TeV_pythia6'
DATANAME = 'PU40bx25-MCRUN2_74_V7-GEN-SIM-RAW-v1'
config.General.requestName = DATASET+'_'+DATANAME

config.section_('JobType')
#config.JobType.psetName = 'QCD_Pt_'+PTHATRANGE+'_bbEnriched_TuneZ2star_13TeV_pythia6_cff_py_GEN_SIM_RAW.py'
config.JobType.psetName = 'QCD_bbEnriched_TuneZ2star_13TeV_pythia6_cff_py_GEN_SIM_RAW.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['pu_sample.py']
config.JobType.outputFiles = ['step2.root']

config.section_('Data')
config.Data.inputDataset = '/QCD_Pt_50to80_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-a72ffaed68cc24200dd6288f95803b9d/USER'
#config.Data.inputDataset = '/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-f56ab72f9958bbc45403b8d1ba752d5f/USER'
#config.Data.inputDataset = '/QCD_Pt_120to170_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-8ae529a7ebd23f0b7f602aecd5540a42/USER'
#config.Data.inputDataset = '/QCD_Pt_170to300_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-a66cf9d8a35cf3f9219038fc2770efe0/USER'
#config.Data.inputDataset = '/QCD_Pt_300to470_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-a71aaff9b91b2a0d3c7e4d400c594e5e/USER'
#config.Data.inputDataset = '/QCD_Pt_470to600_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v2-9d26846b4f3c221f17f69a22b06a7d48/USER'
#config.Data.inputDataset = '/QCD_Pt_600to800_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-f94ac6652c099b47b6ac9194dd438ec2/USER'
#config.Data.inputDataset = '/QCD_Pt_800to1000_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-10db8bada3c451ebc3ee5659196da1c7/USER'
#config.Data.inputDataset = '/QCD_Pt_1000to1400_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-6d10140ffa248da124c63c244d5d9bff/USER'
#config.Data.inputDataset = '/QCD_Pt_1400to1800_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-fb9b0c96c98ab0faa39d9e81d888ad85/USER'
#config.Data.inputDataset = '/QCD_Pt_1800_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-ee09717b96c60ee406c396184e7457b4/USER'

config.Data.unitsPerJob = 1
#NJOBS = 1  # This is not a configuration parameter, but an auxiliary variable used in the next line.
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = True
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.publishDataName = DATANAME
#config.Data.outLFN = '/store/user/rwalsh/tmp'

config.section_('User')
config.User.voGroup = 'dcms'

config.section_('Site')
config.Site.whitelist = ['T2_DE_DESY']
config.Site.storageSite = 'T2_DE_DESY'
