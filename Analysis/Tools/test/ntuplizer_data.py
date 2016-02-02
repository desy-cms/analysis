import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '74X_dataRun2_v5')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

output_file = 'ntuple.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ TRIGGER FILTER ===============
## Enable below at cms.Path if needed 
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
                          "HLT_ZeroBias_v*",
    ),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)


## ============ EVENT FILTER COUNTER ===============
## Filter counter (maybe more useful for MC)
process.TotalEvents    = cms.EDProducer("EventCountProducer")
process.FilteredEvents = cms.EDProducer("EventCountProducer")

## ============ PRIMARY VERTEX FILTER ===============
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"), # primary vertex collection name
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"), # ndof>thr=4 corresponds to sum(track_weigths) > (thr+3)/2 = 3.5 so typically 4 good tracks
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

## ============  THE NTUPLIZER!!!  ===============
process.MssmHbb     = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(False),
    UseFullName     = cms.bool(False),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(
                                    cms.InputTag("slimmedJetsPuppi"),
                                    cms.InputTag("slimmedJets"),
#                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT")
                                    ), 
#     PatMETs         = cms.VInputTag(
#                                     cms.InputTag("slimmedMETs"),
#                                     cms.InputTag("slimmedMETsPuppi")
#                                     ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons")
                                    ), 
    PrimaryVertices = cms.VInputTag(
                                    cms.InputTag("offlineSlimmedPrimaryVertices")
                                    ), 
    BTagAlgorithms = cms.vstring   (
                                    "pfCombinedInclusiveSecondaryVertexV2BJetTags",
#                                     "combinedSecondaryVertexBJetTags",
#                                     "pfJetBProbabilityBJetTags",
#                                     "pfJetProbabilityBJetTags",
#                                     "pfTrackCountingHighPurBJetTags",
#                                     "pfTrackCountingHighEffBJetTags",
#                                     "pfSimpleSecondaryVertexHighEffBJetTags",
#                                     "pfSimpleSecondaryVertexHighPurBJetTags",
#                                     "pfCombinedSecondaryVertexV2BJetTags",
#                                     "pfCombinedSecondaryVertexSoftLeptonBJetTags",
#                                     "pfCombinedMVABJetTags",
                                   ),
    BTagAlgorithmsAlias = cms.vstring   (
                                         "btag_csvivf",
#                                          "btag_csv",
#                                          "btag_jetbprob",
#                                          "btag_jetprob",
#                                          "btag_tchp",
#                                          "btag_tche",
#                                          "btag_svhe",
#                                          "btag_svhp",
#                                          "btag_csvv2",
#                                          "btag_csvlep",
#                                          "btag_csvmva",
                                        ),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
    											"HLT_ZeroBias_v",
                                   ),
#     TriggerObjectStandAlone  = cms.VInputTag(
#                                              cms.InputTag("selectedPatTrigger"),
#                                              ),
#     TriggerObjectLabels    = cms.vstring  (
#     											"hltL1sL1DoubleJetC100",
#                                    ),
   L1ExtraJets     = cms.VInputTag(
                                   cms.InputTag("l1extraParticles","Central","RECO"),
#                                   cms.InputTag("l1extraParticles","Forward","RECO"),
#                                   cms.InputTag("l1extraParticles","Tau","RECO")
                                   ),
   L1ExtraMuons    = cms.VInputTag(
                                   cms.InputTag("l1extraParticles","","RECO")
                                   ),
)

process.p = cms.Path(
                      process.TotalEvents *
                      process.triggerSelection *
                      process.primaryVertexFilter *
                      process.FilteredEvents *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/data/Run2015D/ZeroBias/MINIAOD/05Oct2015-v1/30000/CE916313-4B6F-E511-BD1E-00261894393D.root',
] );


secFiles.extend( [
               ] )
