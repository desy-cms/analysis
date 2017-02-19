import FWCore.ParameterSet.Config as cms

TFileService = cms.Service("TFileService",
	fileName = cms.string('ntuple.root')
)

## ============  THE NTUPLIZER!!!  ===============
ntuplizer           = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(False),
#     ReadPrescale    = cms.bool(False),
#     CrossSection    = cms.double(1.),  # in pb
    UseFullName     = cms.bool(False),
#     ## Monte Carlo only
#     GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer"),
#     GenRunInfo      = cms.InputTag("generator"),
#     GenEventInfo    = cms.InputTag("generator"),
#     PileupInfo      = cms.InputTag("addPileupInfo","","HLT"),
    ###################
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
                                  '',
               ),
    TriggerEvent  = cms.VInputTag(
                     cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                     ),
    TriggerObjectLabels    = cms.vstring  (
                                       '',
               ),
)
