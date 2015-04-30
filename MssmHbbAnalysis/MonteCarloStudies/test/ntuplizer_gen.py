import FWCore.ParameterSet.Config as cms

#Testando 1 2 3

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'PHYS14_25_V1',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

## TFileService
process.TFileService = cms.Service("TFileService",
#	fileName = cms.string('/nfs/dust/cms/user/walsh/analysis/higgs/generated/signal/ntuple_susybbHtobb-M300_genjets.root')
	fileName = cms.string('/nfs/dust/cms/user/walsh/analysis/higgs/generated/background/ntuple_qcd50to80_genjets.root')
)

# GenJets (rerun when not available)
process.load('RecoJets/Configuration/GenJetParticles_cff')
process.load('RecoJets/JetProducers/ak4GenJets_cfi')

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(2.211e07),
    GenJets         = cms.VInputTag(cms.InputTag("ak4GenJets")),
)

#process.p = cms.Path(process.MonteCarloStudies)
process.p = cms.Path(process.genJetParticles+process.ak4GenJets+process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_1.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_10.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_11.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_12.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_13.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_14.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_15.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_16.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_17.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_18.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_19.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_2.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_20.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_21.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_22.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_23.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_24.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_25.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_26.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_27.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_28.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_29.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_3.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_30.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_31.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_32.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_33.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_34.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_35.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_36.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_37.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_38.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_39.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_4.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_40.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_41.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_42.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_43.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_44.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_45.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_46.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_47.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_48.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_49.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_5.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_50.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_6.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_7.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_8.root',
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171406/0000/Producers_9.root' ] );


secFiles.extend( [
               ] )

