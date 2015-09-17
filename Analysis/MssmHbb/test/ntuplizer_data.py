import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
# process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
# process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '74X_dataRun2_Prompt_v1')
# process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
# process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
# for pset in process.GlobalTag.toGet.value():
#     pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
# process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
# process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

output_file = '/nfs/dust/cms/user/walsh/tmp/ntuple_mssmhbb_data_fall15pre4_runs254905to254907.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ TRIGGER FILTER ===============
## Enable below at cms.Path if needed (not recommended for Monte Carlo samples)
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
                          "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v*",
                          "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v*",
                          "HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v*",
                          "HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v*",
                          "HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v*",
                          "HLT_QuadJet45_DoubleBTagCSV0p67_v*",
                          "HLT_QuadJet45_TripleBTagCSV0p67_v*",
                          "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v*",
                          "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v*",
                          "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v*",
                          "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v*",
                          
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
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(
                                    cms.InputTag("slimmedJetsPuppi","","RECO"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO")
                                    ), 
    PatMETs         = cms.VInputTag(
                                    cms.InputTag("slimmedMETs","","RECO"),
                                    cms.InputTag("slimmedMETsPuppi","","RECO")
                                    ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons","","RECO")
                                    ), 
    PrimaryVertices = cms.VInputTag(
                                    cms.InputTag("offlineSlimmedPrimaryVertices","","RECO")
                                    ), 
    BTagAlgorithms = cms.vstring   (
                                    "pfCombinedInclusiveSecondaryVertexV2BJetTags",
                                    "combinedSecondaryVertexBJetTags",
                                    "pfJetBProbabilityBJetTags",
                                    "pfJetProbabilityBJetTags",
                                    "pfTrackCountingHighPurBJetTags",
                                    "pfTrackCountingHighEffBJetTags",
                                    "pfSimpleSecondaryVertexHighEffBJetTags",
                                    "pfSimpleSecondaryVertexHighPurBJetTags",
                                    "pfCombinedSecondaryVertexV2BJetTags",
                                    "pfCombinedSecondaryVertexSoftLeptonBJetTags",
                                    "pfCombinedMVABJetTags",
                                   ),
    BTagAlgorithmsAlias = cms.vstring   (
                                         "btag_csvivf",
                                         "btag_csv",
                                         "btag_jetbprob",
                                         "btag_jetprob",
                                         "btag_tchp",
                                         "btag_tche",
                                         "btag_svhe",
                                         "btag_svhp",
                                         "btag_csvv2",
                                         "btag_csvlep",
                                         "btag_csvmva",
                                        ),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v1",
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v1",
    											"HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v1",
    											"HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v1",
                                    "HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v2",
                                    "HLT_QuadJet45_DoubleBTagCSV0p67_v2",
                                    "HLT_QuadJet45_TripleBTagCSV0p67_v2",
                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v2",
                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v2",
                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v2",
                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v2",
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger","","RECO"),
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
#    L1ExtraJets     = cms.VInputTag(
#                                    cms.InputTag("l1extraParticles","Central","RECO"),
#                                    cms.InputTag("l1extraParticles","Forward","RECO"),
#                                    cms.InputTag("l1extraParticles","Tau","RECO")
#                                    ),
#    L1ExtraMuons    = cms.VInputTag(
#                                    cms.InputTag("l1extraParticles","","RECO")
#                                    ),
)

process.p = cms.Path(
                      process.TotalEvents *
                      process.primaryVertexFilter *
                      process.triggerSelection * 
                      process.FilteredEvents *
                      process.MssmHbb
                    )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
       '/pnfs/desy.de/cms/tier2/store/data/Run2015C/BTagCSV/MINIAOD/PromptReco-v1/000/254/905/00000/1877CA80-B64B-E511-BE62-02163E013611.root',
       '/pnfs/desy.de/cms/tier2/store/data/Run2015C/BTagCSV/MINIAOD/PromptReco-v1/000/254/906/00000/2E43AE21-D74B-E511-B642-02163E011AC9.root',
       '/pnfs/desy.de/cms/tier2/store/data/Run2015C/BTagCSV/MINIAOD/PromptReco-v1/000/254/907/00000/FA67011D-DE4B-E511-A6B5-02163E01437C.root'
] );


secFiles.extend( [
               ] )

