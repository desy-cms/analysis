import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:run2_mc_GRun')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
# fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## TFileService
process.TFileService = cms.Service("TFileService",
#	fileName = cms.string('/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_718_old.root')
	fileName = cms.string('ntuple_btag_filt.root')
)

# trigger filter
process.triggerResultsFilter = cms.EDFilter('TriggerResultsFilter',
   hltResults = cms.InputTag("TriggerResults","","HLT3PB"), # HLT results - set to empty to ignore HLT
   l1tResults = cms.InputTag(""), # L1 GT results - set to empty to ignore L1
   l1tIgnoreMask = cms.bool(False), # use L1 mask
   l1techIgnorePrescales = cms.bool(False), # read L1 technical bits from PSB#9, bypassing the prescales
   daqPartitions = cms.uint32(0x01), # used by the definition of the L1 mask
   throw = cms.bool(False), # throw exception on unknown trigger names
   triggerConditions = cms.vstring( 'HLT_PFJet_BTagCSV_Changed_CSVL3_0p0_v1' )
)
# need some understanding before using; number of jets do not macth the sum.
process.hltL1extraParticles = cms.EDProducer("L1ExtraJetsMerger",
    L1ExtraJets = cms.VInputTag(cms.InputTag("hltL1extraParticles","Tau","HLT3"),cms.InputTag("hltL1extraParticles","Central","HLT3"),cms.InputTag("hltL1extraParticles","Forward","HLT3")),
)   

process.TriggerStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    L1ExtraJets     = cms.VInputTag(cms.InputTag("hltL1extraParticles","Merged","Demo"),
                                    cms.InputTag("hltL1extraParticles","Central","HLT3"),
                                    cms.InputTag("hltL1extraParticles","Tau","HLT3"),
                                    cms.InputTag("hltL1extraParticles","Forward","HLT3")),
    CaloJets        = cms.VInputTag(cms.InputTag("hltAK4CaloJets"),
                                    cms.InputTag("hltAK4CaloJetsCorrected"),
                                    cms.InputTag("hltAK4CaloJetsCorrectedIDPassed"),
                                    cms.InputTag("hltAK4CaloJetsIDPassed")),
    PFJets          = cms.VInputTag(cms.InputTag("hltAK4PFJets"),
                                    cms.InputTag("hltAK4PFJetsCorrected"),
                                    cms.InputTag("hltAK4PFJetsCorrectedLooseID")),
    JetsTags        = cms.VInputTag(cms.InputTag("hltCombinedSecondaryVertexBJetTagsCalo","","HLT3PB"),
                                    cms.InputTag("hltCombinedSecondaryVertexBJetTagsPF","","HLT3PB")),
    GenJets         = cms.VInputTag(cms.InputTag("ak5GenJets")),
    PrimaryVertices = cms.VInputTag(cms.InputTag("hltVerticesPF","","HLT3"),
                                    cms.InputTag("hltVerticesPF","WithBS","HLT3"),
                                    cms.InputTag("hltVerticesPFSelector","","HLT3PB"),
                                    cms.InputTag("hltVerticesPFFilter","","HLT3PB"),
                                    cms.InputTag("hltVerticesL3","","HLT3"),
                                    cms.InputTag("hltVerticesL3","WithBS","HLT3")),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT3"),
                                    cms.InputTag("TriggerResults","","HLT3PB")),
    TriggerPaths    = cms.vstring( "HLT_ZeroBias_v1",
                                   "HLT_PFJet_BTagCSV_v1",
                                   "HLT_PFJet_BTagCSV_Changed_CSVL3_0p0_v1",
                                   "HLT_PFJet_BTagCSV_Changed_CSVL3_0p41_v1",
                                   "HLT_PFJet_BTagCSV_Changed_CSVL3_0p54_v1",
                                   "HLT_PFJet_BTagCSV_Changed_CSVL3_0p63_v1",)
#    PileupInfo = cms.VInputTag(cms.InputTag("addPileupInfo")),
)

process.p = cms.Path(process.triggerResultsFilter*process.hltL1extraParticles*process.TriggerStudies)
#process.p = cms.Path(process.TriggerStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
#readFiles.extend( [
#       'file:Filters.root'
#       'file:/afs/desy.de/user/w/walsh/xxl-af-cms/cms/data/trigger_studies_730_mssmhbb_v7_skim.root'
#       '/store/user/rwalsh/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8/Release718-TriggerStudies_Dev712-OutTriggerOnly-v1/141020_111529/0000/trigger_studies_skim_1.root',
#       '/store/mc/Spring14dr/QCD_Pt-15to3000_Tune4C_Flat_13TeV_pythia8/AODSIM/Flat20to50_POSTLS170_V5-v1/00000/02007198-D1DD-E311-A7A7-E0CB4E29C4D3.root',
#                ] );

#secFiles.extend( [
#               ] )

readFiles.extend( [
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_1.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_10.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_100.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_101.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_102.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_103.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_104.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_105.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_106.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_107.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_108.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_109.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_11.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_110.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_111.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_112.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_113.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_114.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_115.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_116.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_117.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_118.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_119.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_12.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_120.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_121.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_122.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_123.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_124.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_125.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_13.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_14.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_15.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_16.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_17.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_18.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_19.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_2.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_20.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_21.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_22.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_23.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_24.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_25.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_26.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_27.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_28.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_29.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_3.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_30.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_31.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_32.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_33.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_34.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_35.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_36.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_37.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_38.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_39.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_4.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_40.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_41.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_42.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_43.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_44.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_45.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_46.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_47.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_48.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_49.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_5.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_50.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_51.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_52.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_53.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_54.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_55.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_56.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_57.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_58.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_59.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_6.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_60.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_61.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_62.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_63.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_64.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_65.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_66.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_67.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_68.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_69.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_7.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_70.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_71.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_72.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_73.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_74.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_75.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_76.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_77.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_78.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_79.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_8.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_80.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_81.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_82.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_83.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_84.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_85.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_86.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_87.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_88.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_89.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_9.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_90.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_91.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_92.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_93.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_94.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_95.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_96.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_97.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_98.root',
       '/store/user/rwalsh/SUSYBBHToBB_M-300_13TeV-pythia6-tauola/OpenHLT-740-hlt_run2_mssmhbb_scratch_V47-Prod-v2-Filt-v3/150422_115504/0000/Filters_99.root' ] );


secFiles.extend( [
               ] )

