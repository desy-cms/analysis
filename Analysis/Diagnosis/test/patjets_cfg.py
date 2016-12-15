import FWCore.ParameterSet.Config as cms

process = cms.Process("Diagnosis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100000)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_dataRun2_Prompt_ICHEP16JEC_v0')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

## TFileService
output_file = 'histograms_prompt.root'
process.TFileService = cms.Service("TFileService",
   fileName = cms.string(output_file)
)


## ============ TRIGGER FILTER =============== 
## Enable below at cms.Path if needed 
process.trigger = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring('HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v*'),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( False )
)

## ============ RE-APPLY JET ENERGY CORRECTIONS ===============   BE CAREFUL!!!
## Enable below at cms.Path if needed 
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors
process.slimmedJetsCorrFactorsReapplyJEC = updatedPatJetCorrFactors.clone(
  src = cms.InputTag("slimmedJets"),
  levels = ['L1FastJet', 
        'L2Relative', 
        'L3Absolute',
        'L2L3Residual'],
  payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
process.slimmedJetsReapplyJEC = updatedPatJets.clone(
  jetSource = cms.InputTag("slimmedJets"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsCorrFactorsReapplyJEC"))
  )

process.patjets = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJets"),
   PTMin = cms.double(-1),
)
process.patjets20 = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJets"),
   PTMin = cms.double(20.),
)
process.patjets30 = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJets"),
   PTMin = cms.double(30.),
)
process.patjetsJEC = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJetsReapplyJEC"),
   PTMin = cms.double(-1),
)
process.patjetsJEC20 = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJetsReapplyJEC"),
   PTMin = cms.double(20),
)
process.patjetsJEC30 = cms.EDAnalyzer('PatJets',
   PatJets = cms.InputTag("slimmedJetsReapplyJEC"),
   PTMin = cms.double(30),
)

process.p = cms.Path(process.trigger + 
                     process.slimmedJetsCorrFactorsReapplyJEC  + process. slimmedJetsReapplyJEC +
                     process.patjets +
                     process.patjets20 +
                     process.patjets30 +
                     process.patjetsJEC +
                     process.patjetsJEC20 +
                     process.patjetsJEC30
                     )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)




readFiles.extend( [
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/0A806FC7-DD3C-E611-842F-02163E011E20.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/520797DA-DD3C-E611-B1F3-02163E0146ED.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/527B93C9-DD3C-E611-B197-02163E013805.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/52817DC5-DD3C-E611-8B31-02163E01381A.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/9A7777C4-DD3C-E611-A528-02163E013744.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/A45477C4-DD3C-E611-BFFB-02163E01429D.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/A8AD63C1-DD3C-E611-AF0D-02163E011A47.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/A8D18EC7-DD3C-E611-B5B0-02163E011E55.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/B01812A9-ED3C-E611-B748-02163E013440.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/B01D41C6-DD3C-E611-82F1-02163E0122E2.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/B20E19C9-DD3C-E611-87F3-02163E011C30.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/B45EC2C8-DD3C-E611-9D8A-02163E01385D.root',
'/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/DABF59E0-153D-E611-9F7B-02163E01378F.root',
] );


secFiles.extend( [
       ] )



## ============ JSON Certified data ===============   BE CAREFUL!!!
## Don't use with CRAB!!!
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
JSONfile = 'json.txt'
myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
process.source.lumisToProcess.extend(myLumis)
