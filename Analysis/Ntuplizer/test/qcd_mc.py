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
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/0C4916E8-A871-E511-8078-0025905A6126.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/1266D937-AA71-E511-822B-002618943811.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/18F15891-A971-E511-991F-0025905A48F2.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/1C2A68AE-A971-E511-A996-003048FF9AC6.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/1EDB1AC2-A871-E511-8EBF-002618943957.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/2003653D-AA71-E511-B31B-0025905A6094.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/2020205D-A871-E511-A3F7-002590593920.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/22048E39-AA71-E511-B905-0025905B85D6.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/228F5ADE-A871-E511-A9CD-0025905B8576.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/264851E9-A871-E511-A815-002618943834.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/2C0509F2-A771-E511-A3E3-00261894385A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/2C1CDF39-AA71-E511-AC02-0025905A60BE.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3041885F-A871-E511-9D87-003048FFD75C.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/366B475C-A871-E511-8900-0025905964B6.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3A22E2BD-AA71-E511-8C98-0025905A609E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3A790693-AA71-E511-B7FC-0025905B85D8.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3CE16133-A971-E511-9B6D-0025905B85A2.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3E0EA93B-A871-E511-8720-00259059391E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/3EF99808-AB71-E511-AAA5-0025905B858A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/443BCAB7-AA71-E511-BF22-0025905A612A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/443FFA66-A971-E511-AA8B-0025905A48EC.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/482C32A2-A971-E511-A9C7-0025905A60BE.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/4A39B3F0-A771-E511-9E17-002618943974.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/4A456A3A-AA71-E511-99B6-0025905A6088.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/4C11E88F-A971-E511-8B5B-0025905B857C.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/50C4108C-AA71-E511-9FB5-002618943834.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/56318E61-A871-E511-A18A-0025905A610A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/5809FB9B-A971-E511-848A-0025905A60DA.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/5C1F9438-AA71-E511-AD82-002618943896.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/5CC93B20-A871-E511-8513-0025905A48C0.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/5EDC3E52-A971-E511-84EF-0025905A6136.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/64F82F59-A871-E511-B4A1-0025905A48EC.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/66AD8E6A-A971-E511-A4D3-0025905B8562.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/6A588EC4-AA71-E511-AF01-0025905A48F2.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/6AEEFBA3-A971-E511-B17C-002618943868.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/6C1511C1-A871-E511-8D8D-00261894385A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/6C1ED85A-A871-E511-9A05-0025905B85D8.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/72BAFBA7-A971-E511-8D49-0025905B85E8.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/7410003D-A971-E511-B637-0025905B8582.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/7C3ACF3A-AA71-E511-A464-0025905964C4.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/7E8CCBBC-AA71-E511-A0BC-0025905B85E8.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/827CE398-8671-E511-AA37-002590niAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/844A3F81-A971-E511-A159-003048FFD754.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/8A9BBCC6-A871-E511-8C51-0025905A48F2.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/8AB241EE-A871-E511-8BE2-0025905A6088.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/8C5A253C-AA71-E511-9D6D-0025905938D4.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/8CC2C32/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/90A0417C-A971-E511-99E0-0025905A612E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/923E24FD-AA71-E511-A11B-0025905B8596.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_D_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/A6F72F4F-A871-E511-A4A5-0025905B857C.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/A877E850-A871-E511-8CE3-0025905A6056.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/AAE1B302-AB71-E511-8D17-0025905A6056.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/B0D5C679-A971-E511-BA6A-0025905964B6.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/B6331663-A871-E511-96AA-0025905A607A.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt__5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/BA661792-AA71-E511-8E0E-0025905A6088.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/BA6BAF38-A871-E511-945B-0025905A60BE.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to110_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/CC86375B-A871-E511-9E3E-003048FFD754.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/CE4CFA42-A971-E511-8391-0025905A609E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/CEFD2161-A871-E511-9275-0025905A608C.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/D0C2893A-AA71-E511-9C61-0025905B8562.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/D25905B3-A971-E511-B988-0025905A608E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/DADA0B33-A971-E511-8DBB-0025905A609E.root',
       '/store/mc/RunIISpring15MiniAODv2/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/DC2C3EC4-AA71-E511-A7D9-0025905A48EC.root'
] );


secFiles.extend( [
               ] )

