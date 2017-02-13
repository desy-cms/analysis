import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6')
######################################################################
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

output_file = 'ntuple_l1repack.root'
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)


## ============  THE NTUPLIZER!!!  ===============
process.DesyCmsHiggsNtuple     = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    ReadPrescale    = cms.bool(False),
    CrossSection    = cms.double(1.),  # in pb
    UseFullName     = cms.bool(False),
    ## Monte Carlo only
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer"),
    GenRunInfo      = cms.InputTag("generator"),
    GenEventInfo    = cms.InputTag("generator"),
    PileupInfo      = cms.InputTag("addPileupInfo","","HLT"),						
    ###################
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","TEST")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
                                  'HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v',
                                  'HLT_L1SingleJet20_v',
#                                   'HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v',
#                                   'HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v',
#                                   'HLT_DoubleJetsC112_DoubleBTagCSV_p026_DoublePFJetsC172_v',
# #
#                                   'HLT_DoubleJetsC100_SingleBTagCSV_p014_v',
#                                   'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v',
#                                   'HLT_DoubleJetsC100_SingleBTagCSV_p026_v',
#                                   'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v',
# #
#                                   'HLT_PFJet40_v',
#                                   'HLT_PFJet60_v',
#                                   'HLT_PFJet80_v',
#                                   'HLT_PFJet140_v',
#                                   'HLT_PFJet200_v',
               ),
    TriggerEvent  = cms.VInputTag(
                     cms.InputTag("hltTriggerSummaryAOD","","TEST"),
                     ),
    TriggerObjectLabels    = cms.vstring  (
                                       'hltL1sDoubleJetC100',
                                       'hltL1sSingleJet20',
#                                        'hltDoubleJetsC100',
#                                        'hltBTagCaloCSVp014DoubleWithMatching',
#                                        'hltDoublePFJetsC100',
#                                        'hltDoublePFJetsC100MaxDeta1p6',
# #
#                                        'hltBTagCaloCSVp026DoubleWithMatching',
#                                        'hltDoublePFJetsC160',
# #
#                                        'hltL1sDoubleJetC112',
#                                        'hltDoubleJetsC112',
#                                        'hltDoublePFJetsC112',
#                                        'hltDoublePFJetsC112MaxDeta1p6'
# #
#                                        'hltDoublePFJetsC172',
# #                                       
#                                        'hltSingleBTagCSV0p84',
#                                        'hltJetC350',
#                                        'hltSingleBTagCSV0p78',
# #
#                                        'hltL1sZeroBias',
#                                        'hltSingleCaloJet10',
#                                        'hltPFJetsCorrectedMatchedToCaloJets10',
#                                        'hltSinglePFJet40',
# #
#                                        'hltL1sSingleJet35',
#                                        'hltSingleCaloJet40',
#                                        'hltPFJetsCorrectedMatchedToCaloJets40',
#                                        'hltSinglePFJet60',
# #
#                                        'hltL1sSingleJet60',
#                                        'hltSingleCaloJet50',
#                                        'hltPFJetsCorrectedMatchedToCaloJets50',
#                                        'hltSinglePFJet80',
# #
#                                        'hltL1sSingleJet90',
#                                        'hltSingleCaloJet110',
#                                        'hltPFJetsCorrectedMatchedToCaloJets110',
#                                        'hltSinglePFJet140',
# #
#                                        'hltL1sSingleJet120',
#                                        'hltSingleCaloJet170',
#                                        'hltPFJetsCorrectedMatchedToCaloJets170',
#                                        'hltSinglePFJet200',
               ),
)

process.p = cms.Path(
                      process.DesyCmsHiggsNtuple
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       'file:/afs/desy.de/user/w/walsh/cms/analysis/cmssw/hlt/CMSSW_8_0_24/src/HLTrigger/Configuration/mssmhbb/QCD_Pt-120to170_MuEnrichedPt5_User_tranche4_l1repack.root',
#       'file:/nfs/dust/cms/user/vagneria/QCD_Pt-120to170_MuEnrichedPt5_L1_HLT_NoRECO_User_MC.root',
#       'file:/nfs/dust/cms/user/walsh/tmp/qcd_tsg_raw.root',
] );


secFiles.extend( [
               ] )

