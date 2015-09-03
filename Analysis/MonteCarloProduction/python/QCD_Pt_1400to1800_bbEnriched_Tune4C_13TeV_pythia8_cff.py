import FWCore.ParameterSet.Config as cms

generator = cms.EDFilter("Pythia8GeneratorFilter",
	comEnergy = cms.double(13000.0),
	crossSection = cms.untracked.double(7.346e-01),
	filterEfficiency = cms.untracked.double(1),
	maxEventsToPrint = cms.untracked.int32(0),
	pythiaHepMCVerbosity = cms.untracked.bool(False),
	pythiaPylistVerbosity = cms.untracked.int32(0),

	PythiaParameters = cms.PSet(
		processParameters = cms.vstring(
			'Main:timesAllowErrors    = 10000',
			'ParticleDecays:limitTau0 = on',
			'ParticleDecays:tauMax = 10',
			'HardQCD:all = on',
			'PhaseSpace:pTHatMin = 1400  ',
			'PhaseSpace:pTHatMax = 1800  ',
			'Tune:pp 5',
			'Tune:ee 3',

		),
		parameterSets = cms.vstring('processParameters')
	)
)

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/r/rwalsh/cms/cmssw/scratch/CMSSW_7_4_0_pre9/src/MssmHbbAnalysis/Generator/python/QCD_Pt_1400to1800_bbEnriched_Tune4C_13TeV_pythia8_cff.py,v $'),
    annotation = cms.untracked.string('bb-enriched QCD Pt [1400,1800] at 13 TeV')
)

bbFilter = cms.EDFilter("MCParticlePairFilter",
                        Status = cms.untracked.vint32(2,2),
                        MinPt = cms.untracked.vdouble(40.0, 20.0),
                        MaxEta = cms.untracked.vdouble(2.5, 2.5),
                        MinEta = cms.untracked.vdouble(-2.5, -2.5),
                        ParticleID1 = cms.untracked.vint32(-5, 5),
                        ParticleID2 = cms.untracked.vint32(-5, 5)
                      )  

ProductionFilterSequence = cms.Sequence(generator*bbFilter)
