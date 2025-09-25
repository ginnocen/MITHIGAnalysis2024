### HiForest CMSSW Configuration
# Collisions: PbPb UPC (2023)
# Input: miniAOD
# Type: monte carlo

# Era
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_2023_UPC_cff import Run3_2023_UPC
process = cms.Process('HiForest', Run3_2023_UPC)

HIFOREST_VERSION  = '141X'
GLOBAL_TAG        = '141X_dataRun3_v6'
INPUT_TEST_FILE   = '/store/user/yuchenc/UPCD0MC2024PrivateReReco/GNucleusToD0-PhotonBeamB_Pthat0_UPC_5p36TeV_pythia8_evtgen/RECO/250604_221046/0000/step3_100.root'
INPUT_MAX_EVENTS  = 1000
OUTPUT_FILE_NAME  = 'HiForest_PbPbUPC2023_MC.root'

INCLUDE_CENTRALITY_BIN = False
INCLUDE_DEDX_PID  = True
INCLUDE_DFINDER   = True
_DtkPtCut         = 0.2
_DtkEtaCut        = 2.5
INCLUDE_GGNTUPLE  = False
INCLUDE_HLT_OBJ   = False
INCLUDE_JETS      = False
_jetPtMin         = 15
_jetAbsEtaMax     = 2.5
INCLUDE_L1_OBJ    = False
INCLUDE_MUONS     = False
INCLUDE_PF_TREE   = True
_pfPtMin          = 0.01
_pfAbsEtaMax      = 6.
INCLUDE_ZDC       = False

DEBUG             = False

###############################################################################

# HiForest info
process.load('HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi')
process.HiForestInfo.info = cms.vstring('HiForest, miniAOD,' + HIFOREST_VERSION + ', mc')

# load Global Tag, geometry, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, GLOBAL_TAG, '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

###############################################################################

# input files
process.source = cms.Source('PoolSource',
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(INPUT_TEST_FILE),
)

# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(INPUT_MAX_EVENTS)
)

# root output
process.TFileService = cms.Service(
    'TFileService',
    fileName = cms.string(OUTPUT_FILE_NAME)
)

# edm output for debugging purposes
if DEBUG :
    process.output = cms.OutputModule(
        'PoolOutputModule',
        fileName = cms.untracked.string('HiForestEDM.root'),
        outputCommands = cms.untracked.vstring('keep *',)
    )
    process.output_path = cms.EndPath(process.output)

###############################################################################

# Define centrality binning
if INCLUDE_CENTRALITY_BIN :
    process.load('RecoHI.HiCentralityAlgos.CentralityBin_cfi')
    process.centralityBin.Centrality = cms.InputTag('hiCentrality')
    process.centralityBin.centralityVariable = cms.string('HFtowers')

# event analysis
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.hiEvtAnalyzer.doHFfilters = cms.bool(False)
process.hiEvtAnalyzer.doCentrality = cms.bool(False)
process.load('L1Trigger.L1TNtuples.l1MetFilterRecoTree_cfi')
if INCLUDE_PF_TREE :
    process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
    process.particleFlowAnalyser.ptMin = cms.double(_pfPtMin)
    process.particleFlowAnalyser.absEtaMax = cms.double(_pfAbsEtaMax)
if INCLUDE_HLT_OBJ :
    process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
    process.hltobject.triggerNames = cms.vstring()
if INCLUDE_L1_OBJ :
    process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

# electrons, photons, muons
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doGenParticles = cms.bool(True)
process.ggHiNtuplizer.doMuons = cms.bool(False)
process.ggHiNtuplizer.useValMapIso = cms.bool(False) # True here causes seg fault
process.load('TrackingTools.TransientTrack.TransientTrackBuilder_cfi')

# tracks
process.load('HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff')
if INCLUDE_DEDX_PID :
    process.ppTracks.dedxEstimators = cms.VInputTag([
      'dedxEstimator:dedxAllLikelihood',
      'dedxEstimator:dedxPixelLikelihood',
      'dedxEstimator:dedxStripLikelihood'
    ])

# muons
if INCLUDE_MUONS :
    process.load('HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi')
    process.unpackedMuons.muonSelectors = cms.vstring()
    process.load('HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi')
    process.unpackedMuons.muonSelectors = cms.vstring()

# ZDC RecHit Producer && Analyzer
# to prevent crash related to HcalSeverityLevelComputerRcd record
process.load('RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi')
if INCLUDE_ZDC :
    process.load('HeavyIonsAnalysis.ZDCAnalysis.ZDCAnalyzersPbPb_cff')
    process.zdcanalyzer.doHardcodedChargeSum = cms.bool(True)

###############################################################################

# main forest sequence
process.forest = cms.Path(
    process.HiForestInfo +
    process.hltanalysis +
    process.l1MetFilterRecoTree +
    process.trackSequencePP +
    process.hiEvtAnalyzer
)

if INCLUDE_HLT_OBJ :
     process.forest += process.hltobject
if INCLUDE_L1_OBJ :
     process.forest += process.l1object
if INCLUDE_PF_TREE :
    process.forest += process.particleFlowAnalyser
if INCLUDE_CENTRALITY_BIN :
    process.forest += process.centralityBin
if INCLUDE_ZDC :
    process.forest += process.zdcSequencePbPb
if INCLUDE_GGNTUPLE :
    process.forest += process.ggHiNtuplizer
if INCLUDE_MUONS :
    process.forest += process.unpackedMuons
    process.forest += process.muonAnalyzer

###############################################################################

# Select the types of jets filled
_matchJets = True        # Enables q/g and heavy flavor jet identification in MC
# Choose which additional information is added to jet trees
_doHIJetID = True        # Fill jet ID and composition information branches
_doWTARecluster = False  # Add jet phi and eta for WTA axis
_doBtagging  =  False    # Note that setting to True increases computing time a lot
# 0 means use original mini-AOD jets, otherwise use R value, e.g., 3,4,8
# Add all the values you want to process to the list
_jetLabels = ["0"]

if INCLUDE_JETS :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_data_cff')
    # add candidate tagging for all selected jet radii
    from HeavyIonsAnalysis.JetAnalysis.setupJets_ppRef_cff import candidateBtaggingMiniAOD

    for jetLabel in _jetLabels:
        candidateBtaggingMiniAOD(process, isMC = False, jetPtMin = _jetPtMin, jetCorrLevels = ['L2Relative', 'L3Absolute'], doBtagging = _doBtagging, labelR = jetLabel)

        # setup jet analyzer
        setattr(process,"ak"+jetLabel+"PFJetAnalyzer",process.ak4PFJetAnalyzer.clone())
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetTag = "selectedUpdatedPatJetsAK"+jetLabel+"PFCHSBtag"
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetName = 'ak'+jetLabel+'PF'
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchJets = _matchJets
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchTag = 'patJetsAK'+jetLabel+'PFUnsubJets'
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doBtagging = _doBtagging
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doHiJetID = _doHIJetID
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doWTARecluster = _doWTARecluster
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetPtMin = _jetPtMin
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetAbsEtaMax = cms.untracked.double(_jetAbsEtaMax)
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rParam = 0.4 if jetLabel=='0' else float(jetLabel)*0.1
        if doBtagging:
            getattr(process,"ak"+jetLabel+"PFJetAnalyzer").pfJetProbabilityBJetTag = cms.untracked.string("pfJetProbabilityBJetTagsAK"+jetLabel+"PFCHSBtag")
            getattr(process,"ak"+jetLabel+"PFJetAnalyzer").pfUnifiedParticleTransformerAK4JetTags = cms.untracked.string("pfUnifiedParticleTransformerAK4JetTagsAK"+jetLabel+"PFCHSBtag")
        process.forest += getattr(process,"ak"+jetLabel+"PFJetAnalyzer")

###############################################################################

# Event Selection -> add the needed filters here
process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.load('HeavyIonsAnalysis.EventAnalysis.hffilterPF_cfi')
process.pAna = cms.EndPath(process.skimanalysis)

###############################################################################

# D finder
if INCLUDE_DFINDER :
    runOnMC = True
    VtxLabel = 'offlineSlimmedPrimaryVertices'
    TrkLabel = 'packedPFCandidates'
    TrkChi2Label = 'packedPFCandidateTrackChi2'
    GenLabel = 'prunedGenParticles'
    from Bfinder.finderMaker.finderMaker_75X_cff import finderMaker_75X,setCutForAllChannelsDfinder
    finderMaker_75X(process, runOnMC, VtxLabel, TrkLabel, TrkChi2Label, GenLabel)
    process.Dfinder.tkPtCut = cms.double(_DtkPtCut) # before fit
    process.Dfinder.tkEtaCut = cms.double(_DtkEtaCut) # before fit
    process.Dfinder.Dchannel = cms.vint32(
        1, # K+pi- : D0bar
        1, # K-pi+ : D0
        0, # K-pi+pi+ : D+
        0, # K+pi-pi- : D-
        0, # K-pi-pi+pi+ : D0
        0, # K+pi+pi-pi- : D0bar
        0, # K+K-(Phi)pi+ : Ds+
        0, # K+K-(Phi)pi- : Ds-
        0, # D0(K-pi+)pi+ : D+*
        0, # D0bar(K+pi-)pi- : D-*
        0, # D0(K-pi-pi+pi+)pi+ : D+*
        0, # D0bar(K+pi+pi-pi-)pi- : D-*
        0, # D0bar(K+pi+)pi+ : B+
        0, # D0(K-pi+)pi- : B-
        1, # p+k-pi+: lambdaC+
        1  # p-k+pi-: lambdaCbar-
    )
    setCutForAllChannelsDfinder(
        process,
        dPtCut = 0.1,           # Accept if > dPtCut
        VtxChiProbCut = 0.05,   # Accept if > VtxChiProbCut
        svpvDistanceCut = 0.8,  # Accept if < svpvDistanceCut
        alphaCut = 4.           # Accept if < alphaCut (note: 0 < alpha < pi)
    )
    process.Dfinder.dPtCut = cms.vdouble( # Accept if > dPtCut
        0.1, 0.1, 0.,  0.,
        0.,  0.,  0.,  0.,
        0.,  0.,  0.,  0.,
        0.,  0.,  1.,  1.
    )
    process.Dfinder.printInfo = cms.bool(False)
    process.Dfinder.dropUnusedTracks = cms.bool(True)
    process.dfinder = cms.Path(process.DfinderSequence)

###############################################################################

# HLT Filter
# No HLT filter for MC!

if DEBUG :
    process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
        ignoreTotal = cms.untracked.int32(0),
        moduleMemorySummary = cms.untracked.bool(True),
        monitorPssAndPrivate = cms.untracked.bool(False),
        oncePerEventMode = cms.untracked.bool(False)
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
