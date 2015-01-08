import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_718_old.root')
)

# need some understanding before using; number of jets do not macth the sum.
process.l1extraParticles = cms.EDProducer("L1ExtraJetsMerger",
    L1ExtraJets = cms.VInputTag(cms.InputTag("l1extraParticles","Tau","RECO"),cms.InputTag("l1extraParticles","Central","RECO"),cms.InputTag("l1extraParticles","Forward","RECO")),
)   

process.TriggerObjects = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo = cms.bool(True),
    L1ExtraJets = cms.VInputTag(cms.InputTag("l1extraParticles","Merged","Demo"),cms.InputTag("l1extraParticles","Central","RECO"),cms.InputTag("l1extraParticles","Tau","RECO"),cms.InputTag("l1extraParticles","Forward","RECO")),
#    CaloJets = cms.VInputTag(cms.InputTag("hltAK4CaloJets"),cms.InputTag("hltAK4CaloJetsCorrected"),cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),cms.InputTag("hltAK4CaloJetsIDPassed")),
#    PFJets = cms.VInputTag(cms.InputTag("hltAK4PFJets"),cms.InputTag("hltAK4PFJetsCorrected")),
    PatJets = cms.VInputTag(cms.InputTag("slimmedJets")),

#    PileupInfo = cms.VInputTag(cms.InputTag("addPileupInfo")),
)

process.p = cms.Path(process.l1extraParticles*process.TriggerObjects)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/mc/Spring14miniaod/QCD_Pt-30to50_TuneZ2star_13TeV_pythia6/MINIAODSIM/141029_PU40bx50_PLS170_V6AN2-v1/10000/0808D0D0-D065-E411-A928-002590200930.root',
                ] );
readFiles.extend( [
                ] );


secFiles.extend( [
               ] )

