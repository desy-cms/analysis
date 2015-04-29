import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'PHYS14_25_V1',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('ntuple_susybbHtobb-M300_genjets.root')
)

process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    GenJets         = cms.VInputTag(cms.InputTag("ak4GenJets")),
)

process.p = cms.Path(process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/1E5BB9A7-19E9-E411-98AA-0023AEFDEC3C.root',
       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/384C3E92-4FE8-E411-B572-0023AEFDE9C8.root',
       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/C20B0795-2BE8-E411-975F-008CFA1CB55C.root',
       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/E492AEA0-3DE8-E411-AE51-008CFA06477C.root',
] );


secFiles.extend( [
               ] )

