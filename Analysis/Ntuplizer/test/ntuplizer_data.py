import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

#process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '74X_dataRun2_Prompt_v1')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

output_file = 'ntuple_pat.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(False),
    UseFullName     = cms.bool(False),
    L1ExtraJets     = cms.VInputTag(cms.InputTag("l1extraParticles","Central","RECO"),
                                    cms.InputTag("l1extraParticles","Forward","RECO"),
                                    cms.InputTag("l1extraParticles","Tau","RECO")
                                    ),
    L1ExtraMuons    = cms.VInputTag(cms.InputTag("l1extraParticles","","RECO")),
    PatJets         = cms.VInputTag(cms.InputTag("slimmedJetsPuppi","","RECO"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO")
                                    ), 
    PatMuons        = cms.VInputTag(cms.InputTag("slimmedMuons","","RECO")), 
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
    											"HLT_BTagMu_DiJet20_Mu5",
    											"HLT_BTagMu_DiJet40_Mu5",
    											"HLT_BTagMu_DiJet70_Mu5",
    											"HLT_BTagMu_DiJet110_Mu5",
    											"HLT_BTagMu_Jet300_Mu5",
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger","","RECO"),
                                             ),
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

#if options.xsection:
#process.MonteCarloStudies.CrossSection = cms.double(options.xsection)

process.p = cms.Path(process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
       '/store/data/Run2015C/BTagMu/MINIAOD/PromptReco-v1/000/254/319/00000/E68D6A77-AF45-E511-AA2B-02163E01430A.root',
] );


secFiles.extend( [
               ] )

