import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Diagnosis")

options = VarParsing.VarParsing ('analysis')
options.inputFiles =  '/store/data/Run2016H/BTagCSV/MINIAOD/PromptReco-v2/000/282/800/00000/C812D088-0291-E611-832C-FA163EE3836A.root',
options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source ("PoolSource",
                             fileNames      = cms.untracked.vstring (options.inputFiles),
                             )

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_dataRun2_Prompt_v14')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )


process.triggerinfo = cms.EDAnalyzer('TriggerInfo',
   PathName = cms.string("HLT_AK8PFHT750_TrimMass50_v1"),
   TriggerResults = cms.InputTag("TriggerResults","","HLT"),
   TriggerObjectsStandAlone  = cms.InputTag("selectedPatTrigger","","PAT"),
)


process.p = cms.Path(process.triggerinfo)

