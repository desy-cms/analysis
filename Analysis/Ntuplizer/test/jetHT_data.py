import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '74X_dataRun2_Prompt_v1')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

output_file = 'output.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ TRIGGER FILTER ===============
## Enable below at cms.Path if needed (not recommended for Monte Carlo samples)
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
    					  "HLT_PFJet40_v*",
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
    MonteCarlo      = cms.bool(False),
    UseFullName     = cms.bool(False),
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
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
    								"HLT_PFJet40_v",
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
                      process.triggerSelection *
                      process.FilteredEvents *
                      process.MssmHbb
                    )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/0067D1EA-EE6F-E511-B561-0050560207C5.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/00729FDB-706F-E511-8E12-0050560207C5.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/00EE27AF-B16F-E511-A5F4-00259073E382.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/00F77227-DB6F-E511-8221-00259073E3D0.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/00FFECBB-A16F-E511-91F0-00259073E390.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/021DD636-B46F-E511-B192-00259073E382.root',
       '/store/data/Run2015D/JetHT/MINIAOD/05Oct2015-v1/50000/02C56D74-8C6F-E511-8989-00505602078D.root',

] );


secFiles.extend( [
               ] )

