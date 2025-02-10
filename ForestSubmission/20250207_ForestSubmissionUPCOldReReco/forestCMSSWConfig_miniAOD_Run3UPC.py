### HiForest Configuration
# Input: miniAOD
# Type: data or mc
# CMSSW version: CMSSW_13_2_4
# CMSHI version: forest_CMSSW_13_2_X

# THIS IS SET BY submit.sh - DO NOT EDIT!
runYear=2023
isData=1
isMC = not isData

# Additional configs
useHLTFilter = True
useHeavyFlavorJets = False
useDfinder = True
useBfinder = True

###############################################################################
# VERSION CONFIG
###############################################################################

### Era
import FWCore.ParameterSet.Config as cms
process = None
from Configuration.Eras.Era_Run3_2023_UPC_cff import Run3_2023_UPC
process = cms.Process('HiForest', Run3_2023_UPC)

### HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
if isData :
    process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 132X, data")
if isMC :
    process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 132X, mc")

### Global Tag (GT), geometry, sequences, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
from Configuration.AlCa.GlobalTag import GlobalTag
if isData :
    process.GlobalTag = GlobalTag(
        process.GlobalTag,
        '132X_dataRun3_Prompt_v3',
        ''
    )
if isMC :
    process.GlobalTag = GlobalTag(
        process.GlobalTag,
        '132X_mcRun3_2023_realistic_HI_v5',
        ''
    )
    process.GlobalTag.toGet.extend([
        cms.PSet(
            record = cms.string("BTagTrackProbability3DRcd"),
            tag = cms.string("JPcalib_MC94X_2017pp_v2"),
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
        )
    ])
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

###############################################################################
# INPUT / OUTPUT CONFIG
###############################################################################

# Input files (overwritten by CRAB config)
testFile = cms.untracked.vstring('')
if isData :
    # 2023 Data, Jan. 2024 ReReco
    testFile = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/hidata/HIRun2023A/HIForward10/MINIAOD/16Jan2024-v1/2810000/0471153b-76e6-42c8-978f-e3b412782e51.root'
    )
if isMC :
    # 2023 MC, Produced Feb. 2025
    testFile = cms.untracked.vstring(
        '/store/mc/HINPbPbSpring23MiniAOD/GNucleus-QCD-PhotonBeamA_Pthat0_UPC_5p36TeV_pythia8/MINIAODSIM/NoPU_UPC_UPC_132X_mcRun3_2023_realistic_HI_v9-v3/130000/0184b36e-62c7-4683-990c-68d99ba04600.root'
    )
process.source = cms.Source(
    "PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = testFile
)

# Events to process, set -1 to process all (overwritten by CRAB config)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Output file name (preserved by CRAB config)
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("HiForestMiniAOD.root")
)

###############################################################################
# ANALYSIS CONFIG
###############################################################################

### Event analysis
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.hiEvtAnalyzer.Vertex = cms.InputTag('offlineSlimmedPrimaryVertices')
process.hiEvtAnalyzer.doCentrality = cms.bool(False)
process.hiEvtAnalyzer.doHFfilters = cms.bool(False)
process.hiEvtAnalyzer.doMC = cms.bool(False)
if isMC : process.hiEvtAnalyzer.doMC = cms.bool(True)
process.hiEvtAnalyzer.doHiMC = cms.bool(False) # Turn off HI specific MC info
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

# Triggers
from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data_2023_skimmed
from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_mc
if isData : process.hltobject.triggerNames = trigger_list_data_2023_skimmed
if isMC : process.hltobject.triggerNames = trigger_list_mc
process.load('L1Trigger.L1TNtuples.l1MetFilterRecoTree_cfi')

### Particle flow
process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
if isData : process.particleFlowAnalyser.ptMin = cms.double(0.0)
if isMC : process.particleFlowAnalyser.ptMin = cms.double(0.01)

### Gen particles
if isMC :
    process.load('HeavyIonsAnalysis.EventAnalysis.HiGenAnalyzer_cfi')
    process.HiGenParticleAna.ptMin = cms.untracked.double(0.01)
    process.HiGenParticleAna.etaMax = cms.untracked.double(5.1)
    process.HiGenParticleAna.doVertex = cms.untracked.bool(False)

#### Jet reco sequence
if isData :
    process.load("HeavyIonsAnalysis.JetAnalysis.ak2PFJetSequence_ppref_data_cff")
    process.load("HeavyIonsAnalysis.JetAnalysis.ak3PFJetSequence_ppref_data_cff")
    process.load("HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_data_cff")
    process.load("HeavyIonsAnalysis.JetAnalysis.ak4CaloJetSequence_pp_data_cff")
    # The following analyzers are to hack in a calorimeter jet correction
    process.hltAK4CaloRelativeCorrector = cms.EDProducer(
        "LXXXCorrectorProducer",
        algorithm = cms.string('AK4Calo'),
        level = cms.string('L2Relative')
    )
    process.hltAK4CaloAbsoluteCorrector = cms.EDProducer(
        "LXXXCorrectorProducer",
        algorithm = cms.string('AK4Calo'),
        level = cms.string('L3Absolute')
    )
    process.hltAK4CaloCorrector = cms.EDProducer(
        "ChainedJetCorrectorProducer",
        correctors = cms.VInputTag(
            "hltAK4CaloRelativeCorrector",
            "hltAK4CaloAbsoluteCorrector"
        )
    )
    process.hltAK4CaloJetsCorrected = cms.EDProducer(
        "CorrectedCaloJetProducer",
        correctors = cms.VInputTag("hltAK4CaloCorrector"),
        src = cms.InputTag("slimmedCaloJets")
    )
    process.ak4CaloJetAnalyzer.jetTag = cms.InputTag("hltAK4CaloJetsCorrected")
    # End calorimeter jet correction hack
if isMC :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_mc_cff')
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4CaloJetSequence_pp_data_cff')
    process.ak4CaloJetAnalyzer.jetTag = cms.InputTag('slimmedCaloJets')

### Electrons, photons, muons
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doMuons = cms.bool(False) # unpackedMuons collection not found from file
process.ggHiNtuplizer.useValMapIso = cms.bool(False) # True here causes seg fault
process.ggHiNtuplizer.doGenParticles = cms.bool(False)
if isMC : process.ggHiNtuplizer.doGenParticles = cms.bool(True)
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

### Tracks
process.load('HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff')

### Muons analyzer
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")

### ZDC RecHit Producer & Analyzer
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018Producer_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018RecHit_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.zdcanalyzer_cfi')
process.zdcdigi.SOI = cms.untracked.int32(2)
process.zdcanalyzer.doZDCRecHit = False
process.zdcanalyzer.doZDCDigi = True
process.zdcanalyzer.zdcRecHitSrc = cms.InputTag("QWzdcreco")
process.zdcanalyzer.zdcDigiSrc = cms.InputTag("hcalDigis", "ZDC")
process.zdcanalyzer.calZDCDigi = False
process.zdcanalyzer.verbose = False
process.zdcanalyzer.nZdcTs = cms.int32(6)
from CondCore.CondDB.CondDB_cfi import *
process.es_pool = cms.ESSource("PoolDBESSource",
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string("HcalElectronicsMapRcd"),
            tag = cms.string("HcalElectronicsMap_2021_v2.0_data")
        )
    ),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    authenticationMethod = cms.untracked.uint32(1)
)
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')
process.es_ascii = cms.ESSource(
    'HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath("ZDCemap_PbPbUPC2023.txt")
         )
    )
)

###############################################################################
# FORESTING SEQUENCE
###############################################################################

if isData:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hiEvtAnalyzer +
        process.hltanalysis +
        #process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.trackSequencePP +
        process.hltAK4CaloRelativeCorrector +
        process.hltAK4CaloAbsoluteCorrector +
        process.hltAK4CaloCorrector +
        process.hltAK4CaloJetsCorrected +
        process.ak4CaloJetAnalyzer +
        process.particleFlowAnalyser +
        process.ggHiNtuplizer +
        process.zdcanalyzer +
        process.muonSequencePP
    )
if isMC:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hltanalysis *
        process.hiEvtAnalyzer *
        #process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.HiGenParticleAna +
        process.ggHiNtuplizer +
        process.trackSequencePP +
        process.muonSequencePP +
        process.particleFlowAnalyser +
        process.ak4CaloJetAnalyzer
)

###############################################################################
# D/B FINDERS (from Jing Wang @boundino)
###############################################################################

# Setup
if useDfinder or useBfinder :
    AddCaloMuon = False
    runOnMC = isMC
    HIFormat = False
    UseGenPlusSim = False
    VtxLabel = "offlineSlimmedPrimaryVertices"
    TrkLabel = "packedPFCandidates"
    TrkChi2Label = "packedPFCandidateTrackChi2"
    GenLabel = "prunedGenParticles"
    useL1Stage2 = True
    HLTProName = "HLT"
    from Bfinder.finderMaker.finderMaker_75X_cff import finderMaker_75X
    finderMaker_75X(
        process, AddCaloMuon, runOnMC, HIFormat, UseGenPlusSim,
        VtxLabel, TrkLabel, TrkChi2Label, GenLabel, useL1Stage2, HLTProName
    )

### D Finder
if useDfinder :
    process.Dfinder.MVAMapLabel = cms.InputTag(TrkLabel, "MVAValues")
    process.Dfinder.makeDntuple = cms.bool(True)
    process.Dfinder.tkPtCut = cms.double(0.3) # before fit
    process.Dfinder.tkEtaCut = cms.double(2.4) # before fit
    process.Dfinder.Dchannel = cms.vint32(
        1, #RECONSTRUCTION: K+pi- : D0bar
        1, #RECONSTRUCTION: K-pi+ : D0
        0, #RECONSTRUCTION: K-pi+pi+ : D+
        0, #RECONSTRUCTION: K+pi-pi- : D-
        0, #RECONSTRUCTION: K-pi-pi+pi+ : D0
        0, #RECONSTRUCTION: K+pi+pi-pi- : D0bar
        0, #RECONSTRUCTION: K+K-(Phi)pi+ : Ds+
        0, #RECONSTRUCTION: K+K-(Phi)pi- : Ds-
        0, #RECONSTRUCTION: D0(K-pi+)pi+ : D+*
        0, #RECONSTRUCTION: D0bar(K+pi-)pi- : D-*
        0, #RECONSTRUCTION: D0(K-pi-pi+pi+)pi+ : D+*
        0, #RECONSTRUCTION: D0bar(K+pi+pi-pi-)pi- : D-*
        0, #RECONSTRUCTION: D0bar(K+pi+)pi+ : B+
        0, #RECONSTRUCTION: D0(K-pi+)pi- : B-
        0, #RECONSTRUCTION: p+k-pi+: lambdaC+
        0  #RECONSTRUCTION: p-k+pi-: lambdaCbar-
    )
    process.Dfinder.dPtCut = cms.vdouble(
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    )
    process.Dfinder.VtxChiProbCut = cms.vdouble(
        0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    )
    process.Dfinder.dCutSeparating_PtVal = cms.vdouble(
        5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
        5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0
    )
    process.Dfinder.tktkRes_svpvDistanceCut_lowptD = cms.vdouble(
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 0.0, 0.0
    )
    process.Dfinder.tktkRes_svpvDistanceCut_highptD = cms.vdouble(
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 0.0, 0.0
    )
    process.Dfinder.svpvDistanceCut_lowptD = cms.vdouble(
        1.0, 1.0, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.5, 2.5
    )
    process.Dfinder.svpvDistanceCut_highptD = cms.vdouble(
        2.0, 2.0, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.5, 2.5
    )
    process.Dfinder.dropUnusedTracks = cms.bool(True)
    process.Dfinder.detailMode = cms.bool(False)
    process.Dfinder.printInfo = cms.bool(False)
    process.dfinder = cms.Path(process.DfinderSequence)

### B Finder
if useBfinder and isData :
    process.Bfinder.MVAMapLabel = cms.InputTag(TrkLabel,"MVAValues")
    process.Bfinder.makeBntuple = cms.bool(True)
    process.Bfinder.tkPtCut = cms.double(0.3) # before fit
    process.Bfinder.tkEtaCut = cms.double(2.4) # before fit
    process.Bfinder.jpsiPtCut = cms.double(0.0) # before fit
    process.Bfinder.Bchannel = cms.vint32(
        1, #RECONSTRUCTION: J/psi + K
        0, #RECONSTRUCTION: J/psi + Pi
        0, #RECONSTRUCTION: J/psi + Ks
        1, #RECONSTRUCTION: J/psi + K* (K+, Pi-)
        1, #RECONSTRUCTION: J/psi + K* (K-, Pi+)
        1, #RECONSTRUCTION: J/psi + phi
        1  #RECONSTRUCTION: J/psi + pi pi <= psi', X(3872), Bs->J/psi f0
    )
    process.Bfinder.bPtCut = cms.vdouble(
        1.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0
    )
    process.Bfinder.VtxChiProbCut = cms.vdouble(
        0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05
    )
    process.Bfinder.svpvDistanceCut = cms.vdouble(
        2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0
    )
    process.Bfinder.doTkPreCut = cms.bool(True)
    process.Bfinder.doMuPreCut = cms.bool(True)
    process.Bfinder.MuonTriggerMatchingPath = cms.vstring(
        "HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1")
    process.Bfinder.MuonTriggerMatchingFilter = cms.vstring(
        "hltL3f0L3Mu0L2Mu0DR3p5FilteredNHitQ10M1to5")
    process.BfinderSequence.insert(0, process.unpackedMuons)
    process.BfinderSequence.insert(0, process.unpackedTracksAndVertices)
    process.unpackedMuons.muonSelectors = cms.vstring() # uncomment for pp
    process.p = cms.Path(process.BfinderSequence)

###############################################################################
# EVENT SELECTION
###############################################################################

process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.pAna = cms.EndPath(process.skimanalysis)

#### HLT filter
if useHLTFilter and isData :
    HLTPaths = [
        # Double muons
        'HLT_HIUPC_DoubleMuCosmic*_MaxPixelCluster1000_v*',
        'HLT_HIUPC_DoubleMuOpen*_NotMBHF*AND_v*',
        # Not MB
        'HLT_HIUPC_NotMBHF*_v*',
        # Jet triggers
        'HLT_HIUPC_SingleJet*_ZDC1n*XOR_*MaxPixelCluster*',
        'HLT_HIUPC_SingleJet*_NotMBHF2AND_*MaxPixelCluster*',
        # Single muon
        'HLT_HIUPC_SingleMu*_NotMBHF*_MaxPixelCluster*',
        # Single EG
        'HLT_HIUPC_SingleEG3_NotMBHF2AND_v*',
        'HLT_HIUPC_SingleEG5_NotMBHF2AND_v*',
        'HLT_HIUPC_SingleEG3_NotMBHF2AND_SinglePixelTrack_MaxPixelTrack_v*',
        'HLT_HIUPC_SingleEG5_NotMBHF2AND_SinglePixelTrack_MaxPixelTrack_v*',
        # ZDC 1n or, low pixel clusters
        'HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v*',
        'HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v*',
        # ZB, single pixel track
        'HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v*'
    ]
    from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
    process.hltfilter = hltHighLevel.clone(HLTPaths = HLTPaths)
    # True = OR, False = AND between the HLT paths:
    process.hltfilter.andOr = cms.bool(True)
    # Throw exception on unknown path names:
    process.hltfilter.throw = cms.bool(False)

    ### Super filter (events that don't pass filters added here are skipped)
    process.filterSequence = cms.Sequence(
        process.hltfilter
    )
    process.superFilterPath = cms.Path(process.filterSequence)
    process.skimanalysis.superFilters = cms.vstring("superFilterPath")

    # Add filters to start of forest processes
    for path in process.paths :
        getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq
    from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
    process = customiseEarlyDelete(process)
    process.options.wantSummary = cms.untracked.bool(False)
