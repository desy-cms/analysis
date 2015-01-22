import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:run2_mc_GRun')
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
#	fileName = cms.string('/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_718_old.root')
	fileName = cms.string('tests.root')
)

# need some understanding before using; number of jets do not macth the sum.
process.hltL1extraParticles = cms.EDProducer("L1ExtraJetsMerger",
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Tau","HLT3"),cms.InputTag("hltL1extraParticles","Central","HLT3"),cms.InputTag("hltL1extraParticles","Forward","HLT3")),
)   

process.TriggerStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo = cms.bool(True),
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Merged","Demo"),cms.InputTag("hltL1extraParticles","Central","HLT3"),cms.InputTag("hltL1extraParticles","Tau","HLT3"),cms.InputTag("hltL1extraParticles","Forward","HLT3")),
    CaloJets = cms.VInputTag(cms.InputTag("hltAK4CaloJets"),cms.InputTag("hltAK4CaloJetsCorrected"),cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),cms.InputTag("hltAK4CaloJetsIDPassed")),
    PFJets = cms.VInputTag(cms.InputTag("hltAK4PFJets"),cms.InputTag("hltAK4PFJetsCorrected"),cms.InputTag("hltAK4PFJetsCorrectedLooseID")),
    JetsTags = cms.VInputTag(cms.InputTag("hltL3CombinedSecondaryVertexBJetTags")),
    GenJets = cms.VInputTag(cms.InputTag("ak5GenJets")),
    TriggerResults = cms.VInputTag(cms.InputTag("TriggerResults","","HLT3PB"),cms.InputTag("TriggerResults","","HLT3")),
    TriggerPaths = cms.vstring( "HLT_ZeroBias_v1",
                                "HLT_L1DoubleJetC100_v1",
                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_v1",
                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_CSV0p8_v1",
                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_Eta1p7_v1",
                                "HLT_DoubleJet100Eta2p6_DoubleCSV0p5_Changed_Eta1p7_and_CSV0p8_v1")
#    PileupInfo = cms.VInputTag(cms.InputTag("addPileupInfo")),
)

process.p = cms.Path(process.hltL1extraParticles*process.TriggerStudies)
#process.p = cms.Path(process.TriggerStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       'file:Filt.root'
#       'file:/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_730_mssmhbb_v7_skim.root'
#       '/store/user/rwalsh/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8/Release718-TriggerStudies_Dev712-OutTriggerOnly-v1/141020_111529/0000/trigger_studies_skim_1.root',
#       '/store/mc/Spring14dr/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8/AODSIM/Flat20to50_POSTLS170_V5-v1/00000/02007198-D1DD-E311-A7A7-E0CB4E29C4D3.root',
                ] );

secFiles.extend( [
               ] )

