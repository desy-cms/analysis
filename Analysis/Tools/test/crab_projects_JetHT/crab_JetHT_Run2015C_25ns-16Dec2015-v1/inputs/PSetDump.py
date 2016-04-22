import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2015D/JetHT/MINIAOD/16Dec2015-v1/00000/301A497D-70B0-E511-9630-002590D0AFA8.root'),
    secondaryFileNames = cms.untracked.vstring()
)
process.FilteredEvents = cms.EDProducer("EventCountProducer")


process.TotalEvents = cms.EDProducer("EventCountProducer")


process.slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC = cms.EDProducer("PATJetUpdater",
    addJetCorrFactors = cms.bool(True),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsAK8PFCorrFactorsReapplyJEC")),
    jetSource = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring(),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        )
    )
)


process.slimmedJetsAK8PFCorrFactorsReapplyJEC = cms.EDProducer("JetCorrFactorsProducer",
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    flavorType = cms.string('J'),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    payload = cms.string('AK8PFchs'),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    src = cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","RECO"),
    useNPV = cms.bool(True),
    useRho = cms.bool(True)
)


process.slimmedJetsCorrFactorsReapplyJEC = cms.EDProducer("JetCorrFactorsProducer",
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    flavorType = cms.string('J'),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    payload = cms.string('AK4PFchs'),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    src = cms.InputTag("slimmedJets"),
    useNPV = cms.bool(True),
    useRho = cms.bool(True)
)


process.slimmedJetsPuppiCorrFactorsReapplyJEC = cms.EDProducer("JetCorrFactorsProducer",
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    flavorType = cms.string('J'),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    payload = cms.string('AK4PFPuppi'),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    src = cms.InputTag("slimmedJetsPuppi"),
    useNPV = cms.bool(True),
    useRho = cms.bool(True)
)


process.slimmedJetsPuppiReapplyJEC = cms.EDProducer("PATJetUpdater",
    addJetCorrFactors = cms.bool(True),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsPuppiCorrFactorsReapplyJEC")),
    jetSource = cms.InputTag("slimmedJetsPuppi"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring(),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        )
    )
)


process.slimmedJetsReapplyJEC = cms.EDProducer("PATJetUpdater",
    addJetCorrFactors = cms.bool(True),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("slimmedJetsCorrFactorsReapplyJEC")),
    jetSource = cms.InputTag("slimmedJets"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring(),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        )
    )
)


process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    cut = cms.string('!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2'),
    filter = cms.bool(True),
    src = cms.InputTag("offlineSlimmedPrimaryVertices")
)


process.triggerSelection = cms.EDFilter("TriggerResultsFilter",
    daqPartitions = cms.uint32(1),
    hltResults = cms.InputTag("TriggerResults","","HLT"),
    l1tIgnoreMask = cms.bool(False),
    l1tResults = cms.InputTag(""),
    l1techIgnorePrescales = cms.bool(False),
    throw = cms.bool(True),
    triggerConditions = cms.vstring('HLT_PFJet40_v*', 
        'HLT_PFJet60_v*', 
        'HLT_PFJet80_v*', 
        'HLT_PFJet140_v*', 
        'HLT_PFJet200_v*', 
        'HLT_PFJet260_v*', 
        'HLT_PFJet320_v*', 
        'HLT_PFJet400_v*', 
        'HLT_PFJet450_v*', 
        'HLT_PFJet500_v*')
)


process.MssmHbb = cms.EDAnalyzer("Ntuplizer",
    BTagAlgorithms = cms.vstring('pfCombinedInclusiveSecondaryVertexV2BJetTags', 
        'combinedSecondaryVertexBJetTags', 
        'pfJetBProbabilityBJetTags', 
        'pfJetProbabilityBJetTags', 
        'pfTrackCountingHighPurBJetTags', 
        'pfTrackCountingHighEffBJetTags', 
        'pfSimpleSecondaryVertexHighEffBJetTags', 
        'pfSimpleSecondaryVertexHighPurBJetTags', 
        'pfCombinedSecondaryVertexV2BJetTags', 
        'pfCombinedSecondaryVertexSoftLeptonBJetTags', 
        'pfCombinedMVAV2BJetTags'),
    BTagAlgorithmsAlias = cms.vstring('btag_csvivf', 
        'btag_csv', 
        'btag_jetbprob', 
        'btag_jetprob', 
        'btag_tchp', 
        'btag_tche', 
        'btag_svhe', 
        'btag_svhp', 
        'btag_csvv2', 
        'btag_csvlep', 
        'btag_csvmva'),
    CrossSection = cms.double(1),
    FilteredEvents = cms.InputTag("FilteredEvents"),
    JECRecords = cms.vstring('AK4PFchs', 
        'AK4PFPuppi', 
        'AK8PFchs'),
    MonteCarlo = cms.bool(False),
    PatJets = cms.VInputTag(cms.InputTag("slimmedJetsReapplyJEC"), cms.InputTag("slimmedJetsPuppiReapplyJEC"), cms.InputTag("slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC")),
    PatMETs = cms.VInputTag(cms.InputTag("slimmedMETs"), cms.InputTag("slimmedMETsPuppi")),
    PatMuons = cms.VInputTag(cms.InputTag("slimmedMuons")),
    PrimaryVertices = cms.VInputTag(cms.InputTag("offlineSlimmedPrimaryVertices")),
    TotalEvents = cms.InputTag("TotalEvents"),
    TriggerObjectLabels = cms.vstring('hltL1sL1ZeroBias', 
        'hltPFJetsCorrectedMatchedToCaloJets10', 
        'hltSingleCaloJet10', 
        'hltSinglePFJet40', 
        'hltL1sL1SingleJet36', 
        'hltSingleCaloJet40', 
        'hltPFJetsCorrectedMatchedToCaloJets40', 
        'hltSinglePFJet60', 
        'hltL1sL1SingleJet52', 
        'hltPFJetsCorrectedMatchedToCaloJets50', 
        'hltSingleCaloJet50', 
        'hltSinglePFJet80', 
        'hltL1sL1SingleJet92', 
        'hltPFJetsCorrectedMatchedToCaloJets110', 
        'hltSingleCaloJet110', 
        'hltSinglePFJet140', 
        'hltL1sL1SingleJet128', 
        'hltPFJetsCorrectedMatchedToCaloJets170', 
        'hltSingleCaloJet170', 
        'hltSinglePFJet200', 
        'hltL1sL1SingleJet200', 
        'hltPFJetsCorrectedMatchedToCaloJets210', 
        'hltSingleCaloJet210', 
        'hltSinglePFJet260', 
        'hltPFJetsCorrectedMatchedToCaloJets270', 
        'hltSingleCaloJet270', 
        'hltSinglePFJet320', 
        'hltPFJetsCorrectedMatchedToCaloJets350', 
        'hltSingleCaloJet350', 
        'hltSinglePFJet400', 
        'hltPFJetsCorrectedMatchedToCaloJets400', 
        'hltSingleCaloJet400', 
        'hltSinglePFJet450', 
        'hltPFJetsCorrectedMatchedToCaloJets450', 
        'hltSingleCaloJet450', 
        'hltSinglePFJet500'),
    TriggerObjectStandAlone = cms.VInputTag(cms.InputTag("selectedPatTrigger")),
    TriggerPaths = cms.vstring('HLT_PFJet40_v', 
        'HLT_PFJet60_v', 
        'HLT_PFJet80_v', 
        'HLT_PFJet140_v', 
        'HLT_PFJet200_v', 
        'HLT_PFJet260_v', 
        'HLT_PFJet320_v', 
        'HLT_PFJet400_v', 
        'HLT_PFJet450_v', 
        'HLT_PFJet500_v'),
    TriggerResults = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    UseFullName = cms.bool(False)
)


process.p = cms.Path(process.TotalEvents+process.triggerSelection+process.primaryVertexFilter+process.FilteredEvents+process.slimmedJetsCorrFactorsReapplyJEC+process.slimmedJetsReapplyJEC+process.slimmedJetsPuppiCorrFactorsReapplyJEC+process.slimmedJetsPuppiReapplyJEC+process.slimmedJetsAK8PFCorrFactorsReapplyJEC+process.slimmedJetsAK8PFCHSSoftDropPackedReapplyJEC+process.MssmHbb)


process.MessageLogger = cms.Service("MessageLogger",
    FrameworkJobReport = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        optionalPSet = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    ),
    cerr_stats = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        output = cms.untracked.string('cerr'),
        threshold = cms.untracked.string('WARNING')
    ),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
    infos = cms.untracked.PSet(
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        optionalPSet = cms.untracked.bool(True),
        placeholder = cms.untracked.bool(True)
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    )
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('output.root')
)


process.CastorDbProducer = cms.ESProducer("CastorDbProducer")


process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


process.SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0),
    PreFilter = cms.bool(False)
)


process.StripCPEfromTrackAngleESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('StripCPEfromTrackAngle'),
    ComponentType = cms.string('StripCPEfromTrackAngle'),
    parameters = cms.PSet(
        mLC_P0 = cms.double(-0.326),
        mLC_P1 = cms.double(0.618),
        mLC_P2 = cms.double(0.3),
        mTEC_P0 = cms.double(-1.885),
        mTEC_P1 = cms.double(0.471),
        mTIB_P0 = cms.double(-0.742),
        mTIB_P1 = cms.double(0.202),
        mTID_P0 = cms.double(-1.427),
        mTID_P1 = cms.double(0.433),
        mTOB_P0 = cms.double(-1.026),
        mTOB_P1 = cms.double(0.253),
        maxChgOneMIP = cms.double(6000.0),
        useLegacyError = cms.bool(False)
    )
)


process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)


process.siPixelQualityESProducer = cms.ESProducer("SiPixelQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiPixelDetVOffRcd'),
            tag = cms.string('')
        ))
)


process.siStripBackPlaneCorrectionDepESProducer = cms.ESProducer("SiStripBackPlaneCorrectionDepESProducer",
    BackPlaneCorrectionDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    BackPlaneCorrectionPeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    )
)


process.siStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    APVGain = cms.VPSet(cms.PSet(
        Label = cms.untracked.string(''),
        NormalizationFactor = cms.untracked.double(1.0),
        Record = cms.string('SiStripApvGainRcd')
    ), 
        cms.PSet(
            Label = cms.untracked.string(''),
            NormalizationFactor = cms.untracked.double(1.0),
            Record = cms.string('SiStripApvGain2Rcd')
        )),
    AutomaticNormalization = cms.bool(False),
    appendToDataLabel = cms.string(''),
    printDebug = cms.untracked.bool(False)
)


process.siStripLorentzAngleDepESProducer = cms.ESProducer("SiStripLorentzAngleDepESProducer",
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    ),
    LorentzAngleDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripLorentzAngleRcd')
    ),
    LorentzAnglePeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripLorentzAngleRcd')
    )
)


process.siStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('RunInfoRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadStripRcd'),
            tag = cms.string('')
        )),
    PrintDebugOutput = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    appendToDataLabel = cms.string('')
)


process.sistripconn = cms.ESProducer("SiStripConnectivity")


process.stripCPEESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('stripCPE'),
    ComponentType = cms.string('SimpleStripCPE'),
    parameters = cms.PSet(

    )
)


process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        enableConnectionSharing = cms.untracked.bool(True),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0)
    ),
    ReconnectEachRun = cms.untracked.bool(False),
    RefreshEachRun = cms.untracked.bool(False),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    globaltag = cms.string('76X_dataRun2_16Dec2015_v0'),
    pfnPrefix = cms.untracked.string('frontier://FrontierProd/'),
    toGet = cms.VPSet()
)


process.es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    GainWidthsForTrigPrims = cms.bool(False),
    HERecalibration = cms.bool(False),
    HEreCalibCutoff = cms.double(20.0),
    HFRecalibration = cms.bool(False),
    HcalReLabel = cms.PSet(
        RelabelHits = cms.untracked.bool(False),
        RelabelRules = cms.untracked.PSet(
            CorrectPhi = cms.untracked.bool(False),
            Eta1 = cms.untracked.vint32(1, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            Eta16 = cms.untracked.vint32(1, 1, 2, 2, 2, 
                2, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            Eta17 = cms.untracked.vint32(1, 1, 2, 2, 3, 
                3, 3, 4, 4, 4, 
                4, 4, 5, 5, 5, 
                5, 5, 5, 5)
        )
    ),
    hcalTopologyConstants = cms.PSet(
        maxDepthHB = cms.int32(2),
        maxDepthHE = cms.int32(3),
        mode = cms.string('HcalTopologyMode::LHC')
    ),
    iLumi = cms.double(-1.0),
    toGet = cms.untracked.vstring('GainWidths')
)


process.prefer("es_hardcode")

process.CondDBSetup = cms.PSet(
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        enableConnectionSharing = cms.untracked.bool(True),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0)
    )
)

process.HcalReLabel = cms.PSet(
    RelabelHits = cms.untracked.bool(False),
    RelabelRules = cms.untracked.PSet(
        CorrectPhi = cms.untracked.bool(False),
        Eta1 = cms.untracked.vint32(1, 2, 2, 2, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3),
        Eta16 = cms.untracked.vint32(1, 1, 2, 2, 2, 
            2, 2, 2, 2, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3),
        Eta17 = cms.untracked.vint32(1, 1, 2, 2, 3, 
            3, 3, 4, 4, 4, 
            4, 4, 5, 5, 5, 
            5, 5, 5, 5)
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
