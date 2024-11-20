### HiForest Configuration
# Input: miniAOD
# Type: data or mc
# CMSSW version: CMSSW_14_1_4_patch5
# CMSHI version: forest_CMSSW_14_1_X

# THIS IS SET BY submit.sh - DO NOT EDIT!
runYear=2024
isData=1
isMC = not isData

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

### ZDC Calibrations
if isData and runYear == 2024 :
    # NOTE: Only use this starting from run 388000!
    process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')
    process.es_ascii = cms.ESSource(
        'HcalTextCalibrations',
        input = cms.VPSet(
            cms.PSet(
                object = cms.string('Gains'),
                file   = cms.FileInPath(
                    'HeavyIonsAnalysis/Configuration/data/ZDCConditions_1400V/DumpGainsForUpload_AllChannels.txt'
                )
            ),
            cms.PSet(
                object = cms.string('TPChannelParameters'),
                file   = cms.FileInPath(
                    'HeavyIonsAnalysis/Configuration/data/ZDCConditions_1400V/DumpTPChannelParameters_Run387473.txt'
                )
            ),
        )
    )

###############################################################################
# INPUT / OUTPUT
###############################################################################

### Input files (this is overwritten by CRAB config)
process.source = cms.Source(
    "PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        # File from /HIForward0/HIRun2024A-PromptReco-v1/MINIAOD :
        'root://xrootd-cms.infn.it//store/hidata/HIRun2024A/HIForward0/MINIAOD/PromptReco-v1/000/388/000/00000/0df29e8c-46f7-43c2-8f8f-03babc9abe25.root'
    )
)
# Number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

### Output file name (this is preserved by CRAB config)
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
process.hiEvtAnalyzer.doEvtPlane = cms.bool(False)
process.hiEvtAnalyzer.doEvtPlaneFlat = cms.bool(False)
process.hiEvtAnalyzer.doHFfilters = cms.bool(False)
process.hiEvtAnalyzer.doMC = cms.bool(False)
if isMC : process.hiEvtAnalyzer.doMC = cms.bool(True)
process.hiEvtAnalyzer.doHiMC = cms.bool(False) # Turn off HI specific MC info
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')
process.load('L1Trigger.L1TNtuples.l1MetFilterRecoTree_cfi')

### HLT list
if isData and runYear == 2023 :
    from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data_2023
    process.hltobject.triggerNames = trigger_list_data_2023
if isData and runYear == 2024 :
    from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data_2024
    process.hltobject.triggerNames = trigger_list_data_2024

### Particle flow
process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
if isMC : process.particleFlowAnalyser.ptMin = cms.double(0.01)

### Gen particles
if isMC :
    process.load('HeavyIonsAnalysis.EventAnalysis.HiGenAnalyzer_cfi')
    process.HiGenParticleAna.ptMin = cms.untracked.double(0.01)
    process.HiGenParticleAna.etaMax = cms.untracked.double(5.1)
    #process.HiGenParticleAna.doVertex = cms.untracked.bool(True)

### Tracks
process.load('HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff')

### Electrons, photons, muons
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doGenParticles = cms.bool(False)
if isMC : process.ggHiNtuplizer.doGenParticles = cms.bool(True)
process.ggHiNtuplizer.doMuons = cms.bool(False) # unpackedMuons collection not found from file
process.ggHiNtuplizer.useValMapIso = cms.bool(False) # True here causes seg fault
process.load('TrackingTools.TransientTrack.TransientTrackBuilder_cfi')
process.load('HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi')

### Jet reco sequence
if isData :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_data_cff')
if isMC :
    process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_mc_cff')

### ZDC RecHit Producer && Analyzer
# to prevent crash related to HcalSeverityLevelComputerRcd record
process.load('RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.ZDCAnalyzersPbPb_cff')

###############################################################################
# MAIN FOREST SEQUENCE
###############################################################################

if isData:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hiEvtAnalyzer +
        process.hltanalysis +
        process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.ggHiNtuplizer +
        process.trackSequencePP +
        process.zdcSequencePbPb +
        process.muonSequencePP +
        process.particleFlowAnalyser +
        process.zdcanalyzer
    )
if isMC:
    process.forest = cms.Path(
        process.HiForestInfo +
        process.hltanalysis *
        process.hiEvtAnalyzer *
        process.hltobject +
        process.l1object +
        process.l1MetFilterRecoTree +
        process.HiGenParticleAna +
        process.ggHiNtuplizer +
        process.trackSequencePP +
        process.muonSequencePP +
        process.particleFlowAnalyser
)

###############################################################################
# EVENT SELECTION / FILTERING
###############################################################################

### General Filters
process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
#process.load('HeavyIonsAnalysis.EventAnalysis.hffilter_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hffilterPF_cfi')
process.pAna = cms.EndPath(process.skimanalysis)

## Primary vertex filter
#process.primaryVertexFilter = cms.EDFilter(
#    "VertexSelector",
#    src = cms.InputTag("offlineSlimmedPrimaryVertices"),
#    cut = cms.string("!isFake"), #abs(z) <= 25 && position.Rho <= 2"), #in miniADO trackSize()==0, however there is no influence.
#    filter = cms.bool(True), # otherwise it won't filter the event
#)
#process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)

### HLT filter
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(
    HLTPaths = [
        'HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13',
        'HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2'
    ]
)
# True = OR, False = AND between the HLT paths:
process.hltfilter.andOr = cms.bool(True)
# Throw exception on unknown path names:
process.hltfilter.throw = cms.bool(False)

### Super filter (events that don't pass filters added here are skipped)
process.filterSequence = cms.Sequence(
    process.hltfilter #*
#    process.primaryVertexFilter
)
process.superFilterPath = cms.Path(process.filterSequence)
process.skimanalysis.superFilters = cms.vstring("superFilterPath")

# Add filters to start of forest processes
for path in process.paths :
    getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
#process.options.wantSummary = cms.untracked.bool(True)

###############################################################################
# CUSTOMIZATION
###############################################################################

### Jet Reclustering
reclusterJets = False # Set true to recluster (probably not needed)
addR3Jets = False
addR3FlowJets = False
addR4Jets = False
addR4FlowJets = False
addUnsubtractedR4Jets = False
# Select Jets Types
if isData and reclusterJets :
    addR3Jets = True
    addR4Jets = True
if isMC and reclusterJets :
    addR4Jets = True
# Choose additional information added to jet trees
doHIJetID = True             # Fill jet ID and composition information branches
doWTARecluster = True        # Add jet phi and eta for WTA axis
# this is only for non-reclustered jets
addCandidateTagging = False

if addR3Jets or addR3FlowJets or addR4Jets or addR4FlowJets or addUnsubtractedR4Jets :
    # Load
    process.load('HeavyIonsAnalysis.JetAnalysis.extraJets_cff')
    from HeavyIonsAnalysis.JetAnalysis.clusterJetsFromMiniAOD_cff import setupPprefJets
    process.load('HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff')
    # R3 Jets
    if addR3Jets :
        process.jetsR3 = cms.Sequence()
        if isData :
            setupPprefJets(
                'ak3PF', process.jetsR3, process,
                isMC = 0, radius = 0.30, JECTag = 'AK3PF'
            )
        if isMC :
            setupPprefJets(
                'ak03PF', process.jetsR3, process,
                isMC = 1, radius = 0.30, JECTag = 'AK3PF'
            )
        process.ak3PFpatJetCorrFactors.levels = ['L2Relative', 'L3Absolute']
        process.ak3PFpatJetCorrFactors.primaryVertices = 'offlineSlimmedPrimaryVertices'
        process.load('HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff')
        process.ak3PFJetAnalyzer.jetTag = 'ak3PFpatJets'
        process.ak3PFJetAnalyzer.jetName = 'ak3PF'
        process.ak3PFJetAnalyzer.doSubEvent = False # Need to disable this
        if isData :
            process.forest += process.extraJetsData * process.jetsR3 * process.ak3PFJetAnalyzer
        if isMC :
            process.forest += process.extraJetsMC * process.jetsR3 * process.ak3PFJetAnalyzer
    # R4 Jets
    if addR4Jets :
        process.jetsR4 = cms.Sequence()
        if isData :
            setupPprefJets(
                'ak04PF', process.jetsR4, process,
                isMC = 0, radius = 0.40, JECTag = 'AK4PF'
            )
        if isMC :
            setupPprefJets(
                'ak04PF', process.jetsR4, process,
                isMC = 1, radius = 0.40, JECTag = 'AK4PF'
            )
        process.ak04PFpatJetCorrFactors.levels = ['L2Relative', 'L3Absolute']
        process.ak04PFpatJetCorrFactors.primaryVertices = 'offlineSlimmedPrimaryVertices'
        process.load('HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff')
        process.ak4PFJetAnalyzer.jetTag = 'ak04PFpatJets'
        process.ak4PFJetAnalyzer.jetName = 'ak04PF'
        process.ak4PFJetAnalyzer.doSubEvent = False # Need to disable this
        if isData :
            process.forest += process.extraJetsData * process.jetsR4 * process.ak4PFJetAnalyzer
        if isMC :
            process.forest += process.extraJetsMC * process.jetsR4 * process.ak4PFJetAnalyzer
else :
    # If jets are clustered in Reco, can skip to this
    process.forest += process.ak4PFJetAnalyzer

### D Finder
AddCaloMuon = False
runOnMC = False
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
process.Dfinder.MVAMapLabel = cms.InputTag(TrkLabel, "MVAValues")
process.Dfinder.makeDntuple = cms.bool(True)
process.Dfinder.Dchannel = cms.vint32(
    1, #RECONSTRUCTION: K+pi- : D0bar
    1, #RECONSTRUCTION: K-pi+ : D0
    1, #RECONSTRUCTION: K-pi+pi+ : D+
    1, #RECONSTRUCTION: K+pi-pi- : D-
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
process.Dfinder.tkPtCut = cms.double(0.3) # before fit, MC = 0.0?
process.Dfinder.tkEtaCut = cms.double(2.4) # before fit
process.Dfinder.dPtCut = cms.vdouble( # before fit
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
)
process.Dfinder.alphaCut = cms.vdouble(
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
)
process.Dfinder.VtxChiProbCut = cms.vdouble(
    0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05,
    0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05
)
process.Dfinder.dCutSeparating_PtVal = cms.vdouble(
    5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
    5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0
)
process.Dfinder.tktkRes_svpvDistanceCut_lowptD = cms.vdouble(
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
)
process.Dfinder.tktkRes_svpvDistanceCut_highptD = cms.vdouble(
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
)
process.Dfinder.svpvDistanceCut_lowptD = cms.vdouble(
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
)
process.Dfinder.svpvDistanceCut_highptD = cms.vdouble(
    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5
)
process.Dfinder.dropUnusedTracks = cms.bool(True)
process.Dfinder.detailMode = cms.bool(False)
process.Dfinder.printInfo = cms.bool(False)
process.dfinder = cms.Path(process.DfinderSequence)

### B Finder
#AddCaloMuon = False
#runOnMC = False
#HIFormat = False
#UseGenPlusSim = False
#VtxLabel = "offlineSlimmedPrimaryVertices"
#TrkLabel = "packedPFCandidates"
#TrkChi2Label = "packedPFCandidateTrackChi2"
#GenLabel = "prunedGenParticles"
#useL1Stage2 = True
#HLTProName = "HLT"
#
#if isData : # Only works for data
#    process.Bfinder.MVAMapLabel = cms.InputTag(TrkLabel,"MVAValues")
#    process.Bfinder.makeBntuple = cms.bool(True)
#    process.Bfinder.Bchannel = cms.vint32(
#        1, #RECONSTRUCTION: J/psi + K
#        0, #RECONSTRUCTION: J/psi + Pi
#        0, #RECONSTRUCTION: J/psi + Ks
#        1, #RECONSTRUCTION: J/psi + K* (K+, Pi-)
#        1, #RECONSTRUCTION: J/psi + K* (K-, Pi+)
#        1, #RECONSTRUCTION: J/psi + phi
#        1  #RECONSTRUCTION: J/psi + pi pi <= psi', X(3872), Bs->J/psi f0
#    )
#    process.Bfinder.tkPtCut = cms.double(0.8) # before fit
#    process.Bfinder.tkEtaCut = cms.double(2.4) # before fit
#    process.Bfinder.jpsiPtCut = cms.double(0.0) # before fit
#    process.Bfinder.bPtCut = cms.vdouble(
#        1.0, 5.0, 5.0, 5.0, 5.0, 5.0, 1.0
#    ) # before fit
#    process.Bfinder.VtxChiProbCut = cms.vdouble(
#        0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.10
#    )
#    process.Bfinder.svpvDistanceCut = cms.vdouble(
#        2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0
#    )
#    process.Bfinder.doTkPreCut = cms.bool(True)
#    process.Bfinder.doMuPreCut = cms.bool(True)
#    process.Bfinder.MuonTriggerMatchingPath = cms.vstring(
#        "HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v1"
#    )
#    process.Bfinder.MuonTriggerMatchingFilter = cms.vstring(
#        "hltL3f0L3Mu0L2Mu0DR3p5FilteredNHitQ10M1to5"
#    )
#    process.BfinderSequence.insert(0, process.unpackedMuons)
#    process.BfinderSequence.insert(0, process.unpackedTracksAndVertices)
#    #process.BfinderSequence.insert(0, process.muonSequencePP)
#    #process.BfinderSequence.insert(0, process.trackSequencePP)
#    process.unpackedMuons.muonSelectors = cms.vstring()
#    process.Bfinder.printInfo = cms.bool(False)
#
#    process.bfinder = cms.Path(process.BfinderSequence)
