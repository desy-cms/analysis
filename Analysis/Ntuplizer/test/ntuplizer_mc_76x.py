import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_mcRun2_asymptotic_v12')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

output_file = 'test_mc.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
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
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(1),  # in pb
    UseFullName     = cms.bool(False),
    ## Monte Carlo only
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer"),
    GenRunInfo      = cms.InputTag("generator"),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets")),
    GenParticles    = cms.VInputTag(cms.InputTag("prunedGenParticles")),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag( 
                                    cms.InputTag("slimmedJets","","PAT"),
                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT"),
                                    ), 
    JECRecords      = cms.vstring  (
                                    "AK4PFchs",
                                    "AK4PFPuppi",
                                    "AK8PFchs",
                                    ),
    PatMETs         = cms.VInputTag(
                                    cms.InputTag("slimmedMETs","","PAT"),
                                    cms.InputTag("slimmedMETsPuppi","","PAT")
                                    ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons","","PAT")
                                    ), 
    PrimaryVertices = cms.VInputTag(
                                    cms.InputTag("offlineSlimmedPrimaryVertices","","PAT")
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
                                             cms.InputTag("selectedPatTrigger","","PAT"),
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
                      process.primaryVertexFilter *
                      process.FilteredEvents *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/mc/RunIIFall15MiniAODv2/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1Raw_76X_mcRun2_asymptotic_v12-v1/00000/0A91D2C2-12C6-E511-B7E7-90B11C050AD4.root',
] );


secFiles.extend( [
               ] )

