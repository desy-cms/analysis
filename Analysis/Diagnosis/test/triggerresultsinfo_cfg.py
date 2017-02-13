import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Diagnosis")

options = VarParsing.VarParsing ('analysis')
options.inputFiles =  'file:/afs/desy.de/user/w/walsh/cms/analysis/cmssw/hlt/CMSSW_8_0_24/src/HLTrigger/Configuration/mssmhbb/QCD_Pt-120to170_MuEnrichedPt5_User_tranche4_l1repack.root',
#options.inputFiles =  '/store/mc/RunIISummer16DR80/QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/130000/08C6F278-E0A2-E611-BE0C-002590D9DA9C.root',
options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source ("PoolSource",
                             fileNames      = cms.untracked.vstring (options.inputFiles),
                             )

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6')
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:run2_mc_GRun')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )


process.triggerinfo = cms.EDAnalyzer('TriggerResultsInfo',
   TriggerResults = cms.InputTag("TriggerResults","","TEST"),
)


process.p = cms.Path(process.triggerinfo)

