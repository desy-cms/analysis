# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --dasquery file dataset=/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/rwalsh-MCRUN2_74_V7-GEN-SIM-v1-f56ab72f9958bbc45403b8d1ba752d5f/USER instance=prod/phys03 --fileout file:step2.root --pileup_input dbs:/MinBias_TuneA2MB_13TeV-pythia8/Fall13-POSTLS162_V1-v1/GEN-SIM --mc --eventcontent RAWSIM --pileup AVE_40_BX_50ns --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --conditions auto:run2_mc_GRun --step DIGI,L1,DIGI2RAW,HLT --magField 38T_PostLS1 --geometry Extended2015 --no_exec -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_1.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_10.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_100.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_101.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_102.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_103.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_104.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_105.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_106.root', 
        '/store/user/rwalsh/QCD_Pt_80to120_bbEnriched_TuneZ2star_13TeV_pythia6/MCRUN2_74_V7-GEN-SIM-v1/150410_113308/0000/step1_107.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:step2.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(40.000000)
process.mix.bunchspace = cms.int32(50)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
from pu_sample import PUFILES
process.mix.input.fileNames = cms.untracked.vstring(PUFILES)
#process.mix.input.fileNames = cms.untracked.vstring(['/store/mc/Fall13/MinBias_TuneA2MB_13TeV-pythia8/GEN-SIM/POSTLS162_V1-v1/20000/F265F6AD-5D25-E311-B2DC-00145EDD7759.root'])
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_GRun', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.RAWSIMoutput_step])

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

