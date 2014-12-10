import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('output.root')
)

# need some understanding before using; number of jets do not macth the sum.
process.hltL1extraParticles = cms.EDProducer("L1ExtraJetsMerger",
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Tau","TEST"),cms.InputTag("hltL1extraParticles","Central","TEST"),cms.InputTag("hltL1extraParticles","Forward","TEST")),
)   

process.TriggerObjects = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo = cms.bool(True),
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Merged","Demo"),cms.InputTag("hltL1extraParticles","Central","TEST"),cms.InputTag("hltL1extraParticles","Tau","TEST"),cms.InputTag("hltL1extraParticles","Forward","TEST")),
    CaloJets = cms.VInputTag(cms.InputTag("hltAK4CaloJets"),cms.InputTag("hltAK4CaloJetsCorrected"),cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),cms.InputTag("hltAK4CaloJetsIDPassed")),
    PFJets = cms.VInputTag(cms.InputTag("hltAK4PFJets"),cms.InputTag("hltAK4PFJetsCorrected")),
#    PileupInfo = cms.VInputTag(cms.InputTag("addPileupInfo")),
)

process.p = cms.Path(process.hltL1extraParticles*process.TriggerObjects)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/user/rwalsh/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8/Release718-TriggerStudies_Dev712-OutTriggerOnly-v1/141020_111529/0000/trigger_studies_skim_1.root',
                ] );
readFiles.extend( [
                ] );


secFiles.extend( [
               ] )

