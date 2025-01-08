### HiForest Configuration
# Input: miniAOD
# Type: data or mc
# CMSSW version: CMSSW_14_1_4_patch5
# CMSHI version: forest_CMSSW_14_1_X

# THIS IS SET BY submit.sh - DO NOT EDIT!
runYear=2023
isData=1
isMC = not isData

# Additional configs
useHLTFilter = True
useHeavyFlavorJets = True
useDfinder = True
useBfinder = True

###############################################################################
# VERSION CONFIG
###############################################################################

### Era
import FWCore.ParameterSet.Config as cms
process = None
if runYear == 2023 :
    from Configuration.Eras.Era_Run3_2023_UPC_cff import Run3_2023_UPC
    process = cms.Process('HiForest', Run3_2023_UPC)
if runYear == 2024 :
    from Configuration.Eras.Era_Run3_2024_UPC_cff import Run3_2024_UPC
    process = cms.Process('HiForest', Run3_2024_UPC)

### HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
if isData :
    process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 141X, data")
if isMC :
    process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 141X, mc")

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
        '141X_dataRun3_Prompt_v3',
        ''
    )
if isMC :
    process.GlobalTag = GlobalTag(
        process.GlobalTag,
        '141X_mcRun3_2024_realistic_HI_v8',
        ''
    )
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

### 2024 data adjustments
if runYear == 2024 :
    # --> only use this starting from 388000
    process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')
    process.es_ascii = cms.ESSource('HcalTextCalibrations',
       input = cms.VPSet(
          cms.PSet(
             object = cms.string('Gains'),
             file   = cms.FileInPath('HeavyIonsAnalysis/Configuration/data/ZDCConditions_1400V/DumpGainsForUpload_AllChannels.txt')
          ),
          cms.PSet(
            object = cms.string('TPChannelParameters'),
            file   = cms.FileInPath('HeavyIonsAnalysis/Configuration/data/ZDCConditions_1400V/DumpTPChannelParameters_Run387473.txt')
          ),
       )
    )

###############################################################################
# INPUT / OUTPUT CONFIG
###############################################################################

# Input files (overwritten by CRAB config)
testFile = cms.untracked.vstring('')
if runYear == 2023 :
    # 2023 Data, Dec. 2024 ReReco (includes PbPb vertexing)
    testFile = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/hidata/HIRun2023A/HIForward9/MINIAOD/1Dec2024-v1/100000/05c824fb-368e-4090-b305-d007e2750afd.root'
    )
if runYear == 2024 :
    # 2024 Data, Nov. 2024 Prompt Reco
    testFile = cms.untracked.vstring(
        '/store/hidata/HIRun2024A/HIForward0/MINIAOD/PromptReco-v1/000/388/000/00000/0df29e8c-46f7-43c2-8f8f-03babc9abe25.root'
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
#process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')
process.load('L1Trigger.L1TNtuples.l1MetFilterRecoTree_cfi')

### Particle flow
process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
if isMC : process.particleFlowAnalyser.ptMin = cms.double(0.01)

### Gen particles
if isMC :
    process.load('HeavyIonsAnalysis.EventAnalysis.HiGenAnalyzer_cfi')
    process.HiGenParticleAna.ptMin = cms.untracked.double(0.01)
    process.HiGenParticleAna.etaMax = cms.untracked.double(5.1)
    process.HiGenParticleAna.doVertex = cms.untracked.bool(False)

#### Jet reco sequence
if isData :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_data_cff')
if isMC :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_mc_cff')

### Electrons, photons, muons
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doMuons = cms.bool(False) # unpackedMuons collection not found from file
process.ggHiNtuplizer.useValMapIso = cms.bool(False) # True here causes seg fault
process.ggHiNtuplizer.doGenParticles = cms.bool(False)
if isMC : process.ggHiNtuplizer.doGenParticles = cms.bool(True)
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

### Tracks
process.load('HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff')

### Unpacked muons
process.load("HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")

### ZDC RecHit Producer && Analyzer
# to prevent crash related to HcalSeverityLevelComputerRcd record
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")
process.load('HeavyIonsAnalysis.ZDCAnalysis.ZDCAnalyzersPbPb_cff')

###############################################################################
# FORESTING SEQUENCE
###############################################################################

if isData:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hiEvtAnalyzer +
        process.hltanalysis +
#        process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.trackSequencePP +
        process.particleFlowAnalyser +
        process.ggHiNtuplizer +
        process.zdcSequencePbPb +
        process.unpackedMuons +
        process.muonAnalyzer +
        process.zdcanalyzer
    )
if isMC:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hltanalysis *
        process.hiEvtAnalyzer *
#        process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.HiGenParticleAna +
        process.ggHiNtuplizer +
        process.trackSequencePP +
        process.muonSequencePP +
        process.particleFlowAnalyser
)

###############################################################################
# HEAVY FLAVOR JETS (from Hannah Bossi)
###############################################################################

if useHeavyFlavorJets :
    # Select the types of jets filled
    matchJets = True             # Enables q/g and heavy flavor jet identification in MC
    jetPtMin = 15
    jetAbsEtaMax = 2.5

    # Choose which additional information is added to jet trees
    doHIJetID = True             # Fill jet ID and composition information branches
    doWTARecluster = False        # Add jet phi and eta for WTA axis
    doBtagging  =  True         # Note that setting to True increases computing time a lot

    # 0 means use original mini-AOD jets, otherwise use R value, e.g., 3,4,8
    jetLabel = "0"

    # Add candidate tagging, copy/paste to add other jet radii
    from HeavyIonsAnalysis.JetAnalysis.setupJets_ppRef_cff import candidateBtaggingMiniAOD
    candidateBtaggingMiniAOD(
        process,
        isMC = False,
        jetPtMin = jetPtMin,
        jetCorrLevels = ['L2Relative', 'L3Absolute'],
        doBtagging = doBtagging,
        labelR = jetLabel
    )

    # Setup jet analyzer
    setattr(process,"ak"+jetLabel+"PFJetAnalyzer",process.ak4PFJetAnalyzer.clone())
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetTag = 'selectedUpdatedPatJetsDeepFlavour'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetName = 'ak'+jetLabel+'PF'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchJets = matchJets
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchTag = 'patJetsAK'+jetLabel+'PFUnsubJets'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doHiJetID = doHIJetID
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doWTARecluster = doWTARecluster
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetPtMin = jetPtMin
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetAbsEtaMax = cms.untracked.double(jetAbsEtaMax)
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rParam = int(jetLabel)*0.1
    if doBtagging:
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").pfJetProbabilityBJetTag = cms.untracked.string("pfJetProbabilityBJetTagsDeepFlavour")
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").pfUnifiedParticleTransformerAK4JetTags = cms.untracked.string("pfUnifiedParticleTransformerAK4JetTagsDeepFlavour")
    process.forest += getattr(process,"ak"+jetLabel+"PFJetAnalyzer")
else :
    process.forest += process.ak4PFJetAnalyzer

###############################################################################
# D/B FINDERS (from Jing Wang @boundino)
###############################################################################

# Setup
if useDfinder or useBfinder :
    runOnMC = isMC
    VtxLabel = "offlineSlimmedPrimaryVertices"
    TrkLabel = "packedPFCandidates"
    TrkChi2Label = "packedPFCandidateTrackChi2"
    GenLabel = "prunedGenParticles"
    from Bfinder.finderMaker.finderMaker_75X_cff import finderMaker_75X,setCutForAllChannelsDfinder
    finderMaker_75X(process, runOnMC, VtxLabel, TrkLabel, TrkChi2Label, GenLabel)

### D Finder
if useDfinder :
    process.Dfinder.tkPtCut = cms.double(0.05) # before fit
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
    setCutForAllChannelsDfinder(
        process,
        dPtCut = 0.0,
        VtxChiProbCut = 0.05,
        svpvDistanceCut = 1.0,
        alphaCut = 0.5
    )
    process.dfinder = cms.Path(process.DfinderSequence)

### B Finder
if useBfinder :
    process.Bfinder.tkPtCut = cms.double(1.) # before fit
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
    process.Bfinder.bPtCut = cms.vdouble(3.0, 5.0, 5.0, 5.0, 5.0, 5.0, 1.0) # before fit
    process.Bfinder.VtxChiProbCut = cms.vdouble(0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05)
    process.Bfinder.svpvDistanceCut = cms.vdouble(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0)
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
process.load('HeavyIonsAnalysis.EventAnalysis.hffilterPF_cfi')
process.pAna = cms.EndPath(process.skimanalysis)

#### HLT filter
if useHLTFilter :
    HLTPaths = None
    if runYear == 2023 :
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
    if runYear == 2024 :
        HLTPaths = [
            'HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13',
            'HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2'
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
