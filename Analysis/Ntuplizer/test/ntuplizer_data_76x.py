import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_dataRun2_v15')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

output_file = 'test_data.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)


## ============ TRIGGER FILTER =============== 
## Enable below at cms.Path if needed 
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
                        'HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v*',
                        'HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v*',
                        'HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v*',
                        'HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v*',
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
    CrossSection    = cms.double(1),  # in pb
    UseFullName     = cms.bool(False),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(   # Be careful with the correction uncertainties!!!
                                    cms.InputTag("slimmedJets"),
                                    cms.InputTag("slimmedJetsPuppi"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO"),
                                    ), 
    JECRecords      = cms.vstring  (
                                    "AK4PFchs",
                                    "AK4PFPuppi",
                                    "AK8PFchs",
                                    ),
    PatMETs         = cms.VInputTag(
                                    cms.InputTag("slimmedMETs"),
                                    cms.InputTag("slimmedMETsPuppi")
                                    ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons")
                                    ), 
    PrimaryVertices = cms.VInputTag(
                                    cms.InputTag("offlineSlimmedPrimaryVertices")
                                    ), 
    BTagAlgorithms = cms.vstring   (
                                    "pfCombinedInclusiveSecondaryVertexV2BJetTags",
                                   ),
    BTagAlgorithmsAlias = cms.vstring   (
                                         "btag_csvivf",
                                        ),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
    								        'HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v',
    								        'HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v',
    								        'HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v',
    								        'HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v',
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger"),
                                             ),
    TriggerObjectLabels    = cms.vstring  (
    											"hltL1sL1DoubleJetC100",
    											"hltDoubleJetsC100",
    											"hltDoublePFJetsC100",
    											"hltDoublePFJetsC100MaxDeta1p6",
    											"hltDoublePFJetsC160",
    											"hltDoubleBTagCSV0p85",
    											"hltDoubleBTagCSV0p9",
    											"hltL1sL1DoubleJetC112",
    											"hltDoubleJetsC112",
    											"hltDoublePFJetsC112",
    											"hltDoublePFJetsC112MaxDeta1p6",
    											"hltDoublePFJetsC172",
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
       '/store/data/Run2015D/BTagCSV/MINIAOD/16Dec2015-v1/50000/A6D829FA-F6AB-E511-BC84-0090FAA581F4.root',
] );


secFiles.extend( [
               ] )
