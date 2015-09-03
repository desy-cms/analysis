import os
import FWCore.ParameterSet.Config as cms


# options parsing
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.register ('dataset',
				  '',
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.string,
				  "Dataset path")
              
options.register ('ptrange',
				  '',
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.string,
				  "pt-hat range of the QCD samples")
              
options.register ('xsection',
				  -1,
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.float,
				  "Cross section in pb")
              
options.register ('outdir',
				  '/nfs/dust/cms/user/walsh/analysis/mssmhbb/ntuples/miniaod/qcd/full',
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.string,
				  "output directory")
              
options.parseArguments()
#----------------------------------------
dataset = {}
dataset['80to120']    = '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['120to170']   = '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['170to300']   = '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'
dataset['300to470']   = '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['470to600']   = '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'
dataset['600to800']   = '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['800to1000']  = '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'
dataset['1000to1400'] = '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['1400to1800'] = '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['1800to2400'] = '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['2400to3200'] = '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
dataset['3200toInf']  = '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'

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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

output_file = 'ntuple_qcd_pat.root'
if options.ptrange:
   output_file = 'ntuple_qcd_'+options.ptrange+'_pat.root'
if not os.path.exists(options.outdir):
   os.makedirs(options.outdir)
output_file = options.outdir+'/'+output_file
      
## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

# Ntuplizer
process.MonteCarloStudies = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer","","SIM"),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets","","PAT")),
    L1ExtraJets     = cms.VInputTag(cms.InputTag("l1extraParticles","Central","RECO")),
    L1ExtraMuons    = cms.VInputTag(cms.InputTag("l1extraParticles","","RECO")),
    PatJets         = cms.VInputTag(cms.InputTag("slimmedJetsPuppi","","PAT")), 
    PatMuons        = cms.VInputTag(cms.InputTag("slimmedMuons","","PAT")), 
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    											"HLT_ZeroBias_v1",
                                   ),
)

#if options.xsection:
process.MonteCarloStudies.CrossSection = cms.double(options.xsection)

process.p = cms.Path(process.MonteCarloStudies)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)


# Get input files from dataset in DAS
from das_client import get_data

if options.dataset or options.ptrange:
   host    = 'https://cmsweb.cern.ch'
   if options.dataset:
      query = 'file dataset='+options.dataset
   if options.ptrange:
      query = 'file dataset='+dataset[options.ptrange]
   idx     = 0
   limit   = 2
   debug   = False
   data = get_data(host, query, idx, limit, debug, threshold=300, ckey=None, cert=None, das_headers=True)
   for datum in data['data']:
      datafile = str(datum['file'][0]['name'])
      readFiles.extend([datafile])
#else:
#   readFiles.extend(['file:input.root'])

secFiles.extend( [
               ] )

if len(readFiles) < 1:
   print "no input files"
   os._exit(0)
   

