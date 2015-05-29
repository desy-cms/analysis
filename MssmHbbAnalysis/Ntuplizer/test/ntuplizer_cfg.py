import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process("NTP")

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.load("FWCore.MessageService.MessageLogger_cfi")

# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

## TFileService
process.TFileService = cms.Service("TFileService",
#	fileName = cms.string('/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_718_old.root')
	fileName = cms.string('tests.root')
)


# need some understanding before using; number of jets do not macth the sum.
process.hltL1extraParticles = cms.EDProducer("L1ExtraJetsMerger",
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Tau","HLT3"),cms.InputTag("hltL1extraParticles","Central","HLT3"),cms.InputTag("hltL1extraParticles","Forward","HLT3")),
)   

process.TriggerStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo    = cms.bool(True),
#    L1ExtraJets   = cms.VInputTag(cms.InputTag("hltL1extraParticles","Merged","Demo"),
#                                  cms.InputTag("hltL1extraParticles","Central","HLT3"),
#                                  cms.InputTag("hltL1extraParticles","Tau","HLT3"),
#                                  cms.InputTag("hltL1extraParticles","Forward","HLT3")
#                                  ),
    CaloJets      = cms.VInputTag(cms.InputTag("hltAK4CaloJets"),
                                  cms.InputTag("hltAK4CaloJetsCorrected"),
                                  cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),
                                  cms.InputTag("hltAK4CaloJetsIDPassed")
                                  ),
    PFJets        = cms.VInputTag(cms.InputTag("hltAK4PFJets"),
                                  cms.InputTag("hltAK4PFJetsCorrected"),
                                  cms.InputTag("hltAK4PFJetsCorrectedLooseID")
                                  ),
#    JetsTags      = cms.VInputTag(
#                                  cms.InputTag("slimmedJetsPuppi")
#                                  ),
    PatJets       = cms.VInputTag(
                                  cms.InputTag("slimmedJetsPuppi"),
                                  cms.InputTag("slimmedJets"),
#                                  cms.InputTag("selectedPatJetsAK4PFCHS"),
                                  ),
    GenJets       = cms.VInputTag(
                                  cms.InputTag("slimmedGenJets")),
#    TriggerResults = cms.VInputTag(cms.InputTag("TriggerResults","","HLT3PB"),cms.InputTag("TriggerResults","","HLT3")),
#    TriggerPaths = cms.vstring( "HLT_ZeroBias_v1",
#                                "HLT_L1DoubleJetC100_v1",
#                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_v1",
#                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_CSV0p8_v1",
#                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_Eta1p7_v1",
#                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_Eta1p7_and_CSV0p8_v1")
#    PileupInfo = cms.VInputTag(cms.InputTag("addPileupInfo")),
)

#process.p = cms.Path(process.hltL1extraParticles*process.TriggerStudies)
process.p = cms.Path(process.TriggerStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       'file:/tmp/rwalsh/Producers_1.root'
                ] );

secFiles.extend( [
               ] )

