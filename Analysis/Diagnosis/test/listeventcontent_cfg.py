import FWCore.ParameterSet.Config as cms

process = cms.Process("Diagnosis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100000)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_dataRun2_2016SeptRepro_v4')
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_dataRun2_Prompt_ICHEP16JEC_v0')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

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

process.list = cms.EDAnalyzer('ListEventContent',
   PatJets = cms.InputTag("slimmedJets"),
)

process.p = cms.Path(process.trigger + process.slimmedJetsCorrFactorsReapplyJEC  + process. slimmedJetsReapplyJEC + process.list)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
#   '/store/data/Run2016C/BTagCSV/MINIAOD/23Sep2016-v1/70000/0828F9B2-8786-E611-982D-02163E015EA0.root',
 '/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/782/00000/A8AD63C1-DD3C-E611-AF0D-02163E011A47.root',
] );


secFiles.extend( [
       ] )



## ============ JSON Certified data ===============   BE CAREFUL!!!
## Don't use with CRAB!!!
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
JSONfile = 'json_run275782_lumi1.txt'
myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
process.source.lumisToProcess.extend(myLumis)
