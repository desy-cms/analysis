import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2_v1')
######################################################################
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

output_file = 'ntuple.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ RE-APPLY JET ENERGY CORRECTIONS ===============   BE CAREFUL!!!
## Enable below at cms.Path if needed 
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors
process.slimmedJetsCorrFactorsReapplyJEC = updatedPatJetCorrFactors.clone(
  src = cms.InputTag("slimmedJets","","PAT"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!


process.slimmedJetsPuppiCorrFactorsReapplyJEC = updatedPatJetCorrFactors.clone(
  src = cms.InputTag("slimmedJetsPuppi","","PAT"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFPuppi' ) # Make sure to choose the appropriate levels and payload here!
  

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
process.slimmedJetsReapplyJEC = updatedPatJets.clone(
  jetSource = cms.InputTag("slimmedJets","","PAT"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsCorrFactorsReapplyJEC"))
  )

process.slimmedJetsPuppiReapplyJEC = updatedPatJets.clone(
  jetSource = cms.InputTag("slimmedJetsPuppi","","PAT"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsPuppiCorrFactorsReapplyJEC"))
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
    CrossSection    = cms.double(1.),  # in pb
    UseFullName     = cms.bool(False),
    ## Monte Carlo only
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer"),
    GenRunInfo      = cms.InputTag("generator"),
    GenEventInfo    = cms.InputTag("generator"),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets")),
    GenParticles    = cms.VInputTag(cms.InputTag("prunedGenParticles")),
    PileupInfo      = cms.InputTag("slimmedAddPileupInfo"),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(  
                                    cms.InputTag("slimmedJetsReapplyJEC"),
                                    cms.InputTag("slimmedJetsPuppiReapplyJEC"),
                                    ), 
    JECRecords      = cms.vstring  (                       # for the JEC uncertainties
                                    "AK4PFchs",
                                    "AK4PFPuppi",
                                    ),
    # commented to use globaltag, uncomment if using text files                                    
#    JECUncertaintyFiles = cms.vstring  (
#                                    "",
#                                    "",
#                                    ),
    JERRecords      = cms.vstring  (                       # for the JER
                                    "AK4PFchs",
                                    "AK4PFPuppi",
                                    ),
#     JERResFiles     = cms.vstring  (
#                                     "",
#                                     "",
#                                     ),
#     JERSfFiles      = cms.vstring  (
#                                     "",
#                                     "",
#                                     ),
    FixedGridRhoAll = cms.InputTag("fixedGridRhoAll"),
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
                                    "pfJetProbabilityBJetTags",
                                    "pfCombinedMVAV2BJetTags",
                                   ),
    BTagAlgorithmsAlias = cms.vstring   (
                                         "btag_csvivf",
                                         "btag_jetprob",
                                         "btag_csvmva",
                                        ),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
                                  'HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v',
                                  'HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v',
                                  'HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v',
                                  'HLT_DoubleJetsC112_DoubleBTagCSV_p026_DoublePFJetsC172_v',
#
                                  'HLT_DoubleJetsC100_SingleBTagCSV_p014_v',
                                  'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v',
                                  'HLT_DoubleJetsC100_SingleBTagCSV_p026_v',
                                  'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v',
#
                                  'HLT_PFJet40_v',
                                  'HLT_PFJet60_v',
                                  'HLT_PFJet80_v',
                                  'HLT_PFJet140_v',
                                  'HLT_PFJet200_v',
               ),
    TriggerObjectStandAlone  = cms.VInputTag(
                     cms.InputTag("selectedPatTrigger"),
                     ),
    TriggerObjectLabels    = cms.vstring  (
                                       'hltL1sDoubleJetC100',
                                       'hltDoubleJetsC100',
                                       'hltBTagCaloCSVp014DoubleWithMatching',
                                       'hltDoublePFJetsC100',
                                       'hltDoublePFJetsC100MaxDeta1p6',
#
                                       'hltBTagCaloCSVp026DoubleWithMatching',
                                       'hltDoublePFJetsC160',
#
                                       'hltL1sDoubleJetC112',
                                       'hltDoubleJetsC112',
                                       'hltDoublePFJetsC112',
                                       'hltDoublePFJetsC112MaxDeta1p6'
#
                                       'hltDoublePFJetsC172',
#                                       
                                       'hltSingleBTagCSV0p84',
                                       'hltJetC350',
                                       'hltSingleBTagCSV0p78',
#
                                       'hltL1sZeroBias',
                                       'hltSingleCaloJet10',
                                       'hltPFJetsCorrectedMatchedToCaloJets10',
                                       'hltSinglePFJet40',
#
                                       'hltL1sSingleJet35',
                                       'hltSingleCaloJet40',
                                       'hltPFJetsCorrectedMatchedToCaloJets40',
                                       'hltSinglePFJet60',
#
                                       'hltL1sSingleJet60',
                                       'hltSingleCaloJet50',
                                       'hltPFJetsCorrectedMatchedToCaloJets50',
                                       'hltSinglePFJet80',
#
                                       'hltL1sSingleJet90',
                                       'hltSingleCaloJet110',
                                       'hltPFJetsCorrectedMatchedToCaloJets110',
                                       'hltSinglePFJet140',
#
                                       'hltL1sSingleJet120',
                                       'hltSingleCaloJet170',
                                       'hltPFJetsCorrectedMatchedToCaloJets170',
                                       'hltSinglePFJet200',
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
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/user/clange/SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8/SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X/160731_091221/0000/HIG-RunIISpring16MiniAODv2-01205_1.root',
] );


secFiles.extend( [
               ] )

