import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_mcRun2_asymptotic_RunIIFall15DR76_v1')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

output_file = 'output.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

# ## ============ TRIGGER FILTER ===============                   BE CAREFUL!!!
# ## Enable below at cms.Path if needed 
# process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
#     triggerConditions = cms.vstring(
#                           "HLT_ZeroBias_v*",
#     ),
#     hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
#     l1tResults = cms.InputTag( "" ),
#     l1tIgnoreMask = cms.bool( False ),
#     l1techIgnorePrescales = cms.bool( False ),
#     daqPartitions = cms.uint32( 1 ),
#     throw = cms.bool( True )
# )
# 

## ============ RE-APPLY JET ENERGY CORRECTIONS ===============   BE CAREFUL!!!
## Enable below at cms.Path if needed 
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
process.slimmedJetsCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJets","","PAT"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!


process.slimmedJetsPuppiCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJetsPuppi","","PAT"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFPuppi' ) # Make sure to choose the appropriate levels and payload here!
  
process.slimmedJetsAK8PFCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK8PFchs' ) # Make sure to choose the appropriate levels and payload here!
 

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
process.slimmedJetsReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJets","","PAT"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsCorrFactorsReapplyJEC"))
  )

process.slimmedJetsPuppiReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJetsPuppi","","PAT"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsPuppiCorrFactorsReapplyJEC"))
  )

process.slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsAK8PFCorrFactorsReapplyJEC"))
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
    PileupInfo      = cms.InputTag("slimmedAddPileupInfo"),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(   # Be careful with the correction uncertainties!!!
#                                    cms.InputTag("slimmedJets","","PAT"),
#                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
#                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT"),
                                    cms.InputTag("slimmedJetsReapplyJEC"),
                                    cms.InputTag("slimmedJetsPuppiReapplyJEC"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC")
                                    ), 
    JECRecords      = cms.vstring  (
#                                    "",
#                                    "",
#                                    "",
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
                      process.slimmedJetsCorrFactorsReapplyJEC       * process. slimmedJetsReapplyJEC *
                      process.slimmedJetsPuppiCorrFactorsReapplyJEC  * process. slimmedJetsPuppiReapplyJEC *
                      process.slimmedJetsAK8PFCorrFactorsReapplyJEC  * process. slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
                   '/store/mc/RunIIFall15MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0659E975-5BB8-E511-BBD9-C81F66B7ED99.root',
                   '/store/mc/RunIIFall15MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/08F6536B-5CB8-E511-B0F0-0025905A60D0.root',
                   '/store/mc/RunIIFall15MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/10236D50-5BB8-E511-AB57-0025905B8582.root',
                   '/store/mc/RunIIFall15MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1644DB69-5CB8-E511-BD02-0025905A6088.root',
                   '/store/mc/RunIIFall15MiniAODv2/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1657938B-5BB8-E511-AE90-0025905A6066.root',

] );


secFiles.extend( [
               ] )

