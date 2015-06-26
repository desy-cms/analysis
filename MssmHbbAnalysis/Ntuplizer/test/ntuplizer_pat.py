import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'MCRUN2_74_V9',conditions='TrackerAlignmentExtendedError_2011Realistic_v1_mc,TrackerAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonDTAPEObjectsExtended_v0_mc,DTAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+MuonCSCAPEObjectsExtended_v0_mc,CSCAlignmentErrorExtendedRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalSamplesCorrelation_mc,EcalSamplesCorrelationRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseShapes_mc,EcalPulseShapesRcd,frontier://FrontierProd/CMS_CONDITIONS+EcalPulseCovariances_mc,EcalPulseCovariancesRcd,frontier://FrontierProd/CMS_CONDITIONS')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string('ntuple_pat_2.root')
)

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(3.0001143e06),
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer","","SIM"),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets","","PAT")),
    L1ExtraJets     = cms.VInputTag(cms.InputTag("l1extraParticles","Central","RECO"),
                                    cms.InputTag("l1extraParticles","Tau","RECO")),
    L1ExtraMuons    = cms.VInputTag(cms.InputTag("l1extraParticles","","RECO")),
    PatJets         = cms.VInputTag(cms.InputTag("slimmedJets","","PAT"),
                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT")), 
    PatMuons        = cms.VInputTag(cms.InputTag("slimmedMuons","","PAT")), 
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    											"HLT_BTagMu_DiJet20_Mu5",
    											"HLT_BTagMu_DiJet40_Mu5",
    											"HLT_BTagMu_DiJet70_Mu5",
    											"HLT_BTagMu_DiJet110_Mu5",
    											"HLT_BTagMu_Jet300_Mu5",
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
    TriggerObjectStandAlone  = cms.InputTag("selectedPatTrigger","","PAT"),
    TriggerObjectLabels    = cms.vstring  (
    											"hltSinglePFJet40",
    											"hltSinglePFJet60",
    											"hltSinglePFJet80",
    											"hltSingleCaloJet10",
    											"hltSingleCaloJet40",
    											"hltL1sL1SingleMu5",
    											"hltL1sMu5",
                                   ),
)

process.p = cms.Path(process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
#       '/store/mc/RunIISpring15DR74/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/2A98D4CF-F9FE-E411-AB1A-047D7BD6DD44.root',
       '/store/mc/RunIISpring15DR74/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/4839E86C-B2FE-E411-A854-0025B3E05C74.root',
#       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/C20B0795-2BE8-E411-975F-008CFA1CB55C.root',
#       '/store/mc/RunIIWinter15GS/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/E492AEA0-3DE8-E411-AE51-008CFA06477C.root',
] );


secFiles.extend( [
               ] )

