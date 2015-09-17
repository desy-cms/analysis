import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
# process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
# process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'MCRUN2_74_V9')
# process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
# process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
# for pset in process.GlobalTag.toGet.value():
#     pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
# process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
# process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

output_file = '/nfs/dust/cms/user/walsh/tmp/ntuple_mssmhbb_data_m300.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

# Ntuplizer
process.MssmHbb     = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(1),  # in pb
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer","","SIM"),
    UseFullName     = cms.bool(False),
#    EventFilter     = cms.VInputTag(cms.InputTag("nEventsTotal"),
#                                    cms.InputTag("nEventsFiltered")),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets","","PAT")),
    GenParticles    = cms.VInputTag(cms.InputTag("prunedGenParticles","","PAT")),
    PatJets         = cms.VInputTag(
                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT")
                                    ), 
    PatMETs         = cms.VInputTag(
                                    cms.InputTag("slimmedMETs","","PAT"),
                                    cms.InputTag("slimmedMETsPuppi","","PAT")
                                    ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons","","PAT")
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
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v1",
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v1",
    											"HLT_DoubleJetsC112_DoubleBTagCSV0p9_DoublePFJetsC112MaxDeta1p6_v1",
    											"HLT_DoubleJetsC112_DoubleBTagCSV0p85_DoublePFJetsC172_v1",
                                    "HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v2",
                                    "HLT_QuadJet45_DoubleBTagCSV0p67_v2",
                                    "HLT_QuadJet45_TripleBTagCSV0p67_v2",
                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v2",
                                    "HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v2",
                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v2",
                                    "HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v2",
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger","","PAT"),
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
#                      process.EventsTotal *
#                      process.triggerSelection * 
#                      process.EventsFiltered *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/mc/RunIISpring15DR74/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/50000/38C86330-3F13-E511-86E6-002590A37106.root',
] );


secFiles.extend( [
               ] )

