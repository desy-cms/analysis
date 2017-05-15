import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_dataRun2_16Dec2015_v0')
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



## ============ RE-APPLY JET ENERGY CORRECTIONS ===============   BE CAREFUL!!!
## Enable below at cms.Path if needed 
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
process.slimmedJetsCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJets"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!


process.slimmedJetsPuppiCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJetsPuppi"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK4PFPuppi' ) # Make sure to choose the appropriate levels and payload here!
  
process.slimmedJetsAK8PFCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
  src = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO"),
  levels = ['L1FastJet', 
            'L2Relative', 
            'L3Absolute'],
  payload = 'AK8PFchs' ) # Make sure to choose the appropriate levels and payload here!
 

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
process.slimmedJetsReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJets"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsCorrFactorsReapplyJEC"))
  )

process.slimmedJetsPuppiReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJetsPuppi"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsPuppiCorrFactorsReapplyJEC"))
  )

process.slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC = patJetsUpdated.clone(
  jetSource = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO"),
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

## ============= Jet Energy Resolution =============
## DOESN'T WORK FOR PUPPI!
'''
process.load('Configuration.StandardSequences.Services_cff')
process.load("JetMETCorrections.Modules.JetResolutionESProducer_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *

process.jer = cms.ESSource("PoolDBESSource",
        CondDBSetup,
        toGet = cms.VPSet(
            # Resolution
            cms.PSet(
                record = cms.string('JetResolutionRcd'),
                tag    = cms.string('JR_Fall15_25nsV2_DATA_PtResolution_AK4PFchs'),
                label  = cms.untracked.string('AK4PFchs_pt')
                ),

            # Scale factors
            cms.PSet(
                record = cms.string('JetResolutionScaleFactorRcd'),
                tag    = cms.string('JR_Fall15_25nsV2_DATA_SF_AK4PFchs'),
                label  = cms.untracked.string('AK4PFchs')
                ),
            ),
        connect = cms.string('sqlite:jer_files/Fall15_25nsV2_DATA.db')
        )

process.es_prefer_jer = cms.ESPrefer('PoolDBESSource', 'jer')
'''
##
## ============  THE NTUPLIZER!!!  ===============
process.MssmHbb     = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(False),
    CrossSection    = cms.double(1),  # in pb
    UseFullName     = cms.bool(False),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    JERResFiles		= cms.vstring(
#    								"",
#    								"",
#    								""
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_PtResolution_AK4PFchs.txt",
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_PtResolution_AK4PFPuppi.txt",
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_PtResolution_AK8PFchs.txt",
    								),
    JERSfFiles		= cms.vstring(
#    								"",
#    								"",
#    								""
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_SF_AK4PFchs.txt",
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_SF_AK4PFPuppi.txt",
    								os.environ["CMSSW_BASE"] + "/src/Analysis/Ntuplizer/test/jer_files/Fall15_25nsV2_DATA_SF_AK8PFchs.txt"
    								),
    PatJets         = cms.VInputTag(   # Be careful with the correction uncertainties!!!
#                                    cms.InputTag("slimmedJets","","PAT"),
#                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
#                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT"),
                                    cms.InputTag("slimmedJetsReapplyJEC"),
                                    cms.InputTag("slimmedJetsPuppiReapplyJEC"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC")
                                    ), 
    JERRecords		= cms.vstring  (
    #                               "",
#                                   "",
#                                   "",
                                    "AK4PFchs",
                                    "AK4PFPuppi",
                                    "AK8PFchs",
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
                      process.triggerSelection *
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
       '/store/data/Run2015D/BTagCSV/MINIAOD/16Dec2015-v1/50000/A6D829FA-F6AB-E511-BC84-0090FAA581F4.root',
] );


secFiles.extend( [
               ] )

## ============ JSON Certified data ===============   BE CAREFUL!!!
## Don't use with CRAB!!!
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
JSONfile = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
process.source.lumisToProcess.extend(myLumis)

