import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'MCRUN2_74_V9')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(150000) )

output_file = 'output.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ TRIGGER FILTER ===============
## Enable below at cms.Path if needed 
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
                          "HLT_PFJet60_v*",
                          "HLT_PFJet80_v*",
                          "HLT_PFJet140_v*",
                          "HLT_PFJet200_v*",
                          "HLT_PFJet260_v*",
                          "HLT_PFJet320_v*",
                          "HLT_PFJet400_v*",
                          "HLT_PFJet450_v*",
                          "HLT_PFJet500_v*"
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
                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT")
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
 #   											"HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v",
#    											"HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v",
#    											"HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v",
#    											"HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v",
#                                    "HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v",
#                                    "HLT_QuadJet45_DoubleBTagCSV0p67_v",
#                                    "HLT_QuadJet45_TripleBTagCSV0p67_v",
#                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",
#                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v",
#                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",
#                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v",
                                    "HLT_PFJet60_v",
                          			"HLT_PFJet80_v",
                          			"HLT_PFJet140_v",
                          			"HLT_PFJet200_v",
                          			"HLT_PFJet260_v",
                          			"HLT_PFJet320_v",
                          			"HLT_PFJet400_v",
                          			"HLT_PFJet450_v",
                          			"HLT_PFJet500_v"
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger","","PAT"),
                                             ),
    TriggerObjectLabels    = cms.vstring  (
#    											"hltL1sL1DoubleJetC100",
#    											"hltDoubleJetsC100",
#    											"hltDoublePFJetsC100",
#    											"hltDoublePFJetsC100MaxDeta1p6",
#    											"hltDoublePFJetsC160",
#    											"hltDoubleBTagCSV0p85",
#    											"hltDoubleBTagCSV0p9",
#    											"hltL1sL1DoubleJetC112",
#    											"hltDoubleJetsC112",
#    											"hltDoublePFJetsC112",
#    											"hltDoublePFJetsC112MaxDeta1p6",
#    											"hltDoublePFJetsC172",
#    											
    											"hltL1sL1ZeroBias",
                                                "hltPFJetsCorrectedMatchedToCaloJets10",
												"hltSingleCaloJet10",
                                                "hltSinglePFJet40",
                                                                                        "hltL1sL1SingleJet36",
																						"hltSingleCaloJet40",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets40",
                                                                                        "hltSinglePFJet60",
                                                                                        "hltL1sL1SingleJet52",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets50",
																						"hltSingleCaloJet50",                                                                                        
																						"hltSinglePFJet80",
                                                                                        "hltL1sL1SingleJet92",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets110",
																						"hltSingleCaloJet110",
                                                                                        "hltSinglePFJet140",
                                                                                        "hltL1sL1SingleJet128",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets170",
																						"hltSingleCaloJet170",
                                                                                        "hltSinglePFJet200",
                                                                                        "hltL1sL1SingleJet200",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets210",
																						"hltSingleCaloJet210",
                                                                                        "hltSinglePFJet260",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets270",
																						"hltSingleCaloJet270",
                                                                                        "hltSinglePFJet320",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets350",
																						"hltSingleCaloJet350",
                                                                                        "hltSinglePFJet400",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets400",
																						"hltSingleCaloJet400",
                                                                                        "hltSinglePFJet450",
                                                                                        "hltPFJetsCorrectedMatchedToCaloJets450",
																						"hltSingleCaloJet450",
                                                                                        "hltSinglePFJet500",
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
                      process.FilteredEvents *
#                      process.triggerSelection *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
'/store/mc/RunIIFall15MiniAODv2/QCDbbbar_Pt-15to3000_TuneZ2star_Flat_13TeV_pythia6/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/3EFBAE60-3BBC-E511-A73A-1CC1DE18CE9C.root',
       '/store/mc/RunIIFall15MiniAODv2/QCDbbbar_Pt-15to3000_TuneZ2star_Flat_13TeV_pythia6/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/547D73D0-90BD-E511-AEDD-D4AE526A0CE0.root',
       '/store/mc/RunIIFall15MiniAODv2/QCDbbbar_Pt-15to3000_TuneZ2star_Flat_13TeV_pythia6/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/62EBC254-29BC-E511-A28A-001EC9AF1FBF.root'
] );


secFiles.extend( [
               ] )

