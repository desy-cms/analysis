import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'MCRUN2_74_V9',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('ntuple_openhlt.root')
)

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(1),
    L1ExtraJets     = cms.VInputTag(
                                    cms.InputTag("hltL1extraParticles","Central","HLT3"),
                                    cms.InputTag("hltL1extraParticles","Tau","HLT3"),
                                    cms.InputTag("hltL1extraParticles","Forward","HLT3")
                                   ),
    CaloJets        = cms.VInputTag(
                                    cms.InputTag("hltAK4CaloJets"),
                                    cms.InputTag("hltAK4CaloJetsCorrected"),
                                    cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),
                                    cms.InputTag("hltAK4CaloJetsIDPassed")
                                   ),
    PFJets          = cms.VInputTag(
                                    cms.InputTag("hltAK4PFJets"),
                                    cms.InputTag("hltAK4PFJetsCorrected"),
                                    cms.InputTag("hltAK4PFJetsCorrectedLooseID")
                                   ),
    JetsTags        = cms.VInputTag(
                                    cms.InputTag("hltCombinedSecondaryVertexBJetTagsCalo","","HLT3"),
                                   ),
    GenJets         = cms.VInputTag(
                                    cms.InputTag("ak4GenJets")
                                   ),
    TriggerResults  = cms.VInputTag(
                                    cms.InputTag("TriggerResults","","HLT3")
                                   ),
    TriggerPaths    = cms.vstring  ( 
                                    "HLT_DoubleJet84Eta2p6_DoubleBTagCSV0p5_v1",
                                   ),
)

process.p = cms.Path(process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/user/rwalsh/QCD_Pt-50to80_Tune4C_13TeV_pythia8/OpenHLT-744_run2_mc_GRun-hlt_run2_mssmhbb_scratch_V59b-Producers-TriggerFilter-v2/150609_121646/0000/Producers_1.root',
] );


secFiles.extend( [
               ] )

