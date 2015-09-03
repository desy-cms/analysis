import FWCore.ParameterSet.Config as cms

#Testando 1 2 3

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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

## TFileService
process.TFileService = cms.Service("TFileService",
#	fileName = cms.string('/nfs/dust/cms/user/walsh/analysis/higgs/generated/signal/ntuple_susybbHtobb-M1100_genjets_v2.root')
	fileName = cms.string('/nfs/dust/cms/user/walsh/analysis/higgs/generated/background/ntuple_qcd470to600_genjets.root')
)

# GenJets (rerun when not available)
process.load('RecoJets/Configuration/GenJetParticles_cff')
process.load('RecoJets/JetProducers/ak4GenJets_cfi')

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(5.871e02),
    GenJets         = cms.VInputTag(cms.InputTag("ak4GenJets")),
)

#process.p = cms.Path(process.MonteCarloStudies)
process.p = cms.Path(process.genJetParticles+process.ak4GenJets+process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
# readFiles.extend( [
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/1E5BB9A7-19E9-E411-98AA-0023AEFDEC3C.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/384C3E92-4FE8-E411-B572-0023AEFDE9C8.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/C20B0795-2BE8-E411-975F-008CFA1CB55C.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/E492AEA0-3DE8-E411-AE51-008CFA06477C.root',
# ] );

# readFiles.extend( [
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/0ACCBA7A-11E8-E411-9D00-008CFA0A5740.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/14A46C14-07E8-E411-B659-0023AEFDE878.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/4EF1FC6A-07E8-E411-A149-0023AEFDEEA4.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/5C7B0EBF-FFE7-E411-A017-008CFA197EB0.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/66D099EB-15E8-E411-8C9D-0023AEFDEE24.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/8A0D5B2F-05E8-E411-A717-0023AEFDE264.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/B02492D1-25E8-E411-8DB2-008CFA0A5740.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/E22306A6-10E8-E411-B606-0023AEFDF04C.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/FED932CB-0AE8-E411-A9C6-0023AEFDE27C.root',
# 
# ] );

# readFiles.extend( [
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/7010E2BF-F4E8-E411-9FE2-008CFA064778.root',
#        '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/80000/900BE68A-29E8-E411-B89D-0023AEFDEBCC.root',
# ] );

readFiles.extend( [
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_1.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_10.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_11.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_12.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_13.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_14.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_15.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_16.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_17.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_18.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_19.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_2.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_20.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_21.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_22.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_23.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_24.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_25.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_26.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_27.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_28.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_29.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_3.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_30.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_31.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_32.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_33.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_34.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_35.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_36.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_37.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_38.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_39.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_4.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_40.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_41.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_42.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_43.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_44.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_45.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_46.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_47.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_48.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_49.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_5.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_50.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_6.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_7.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_8.root',
       '/store/user/rwalsh/QCD_Pt-470to600_Tune4C_13TeV_pythia8/OpenHLT-740p1_MCRUN2_72_V3-hlt_run2_mssmhbb_scratch_V47-Producers-v1/150421_171538/0000/Producers_9.root' ] );



secFiles.extend( [
               ] )

