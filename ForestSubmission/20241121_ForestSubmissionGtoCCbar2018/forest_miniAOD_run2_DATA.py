### HiForest Configuration
# Input: miniAOD
# Type: data

import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run2_2018_pp_on_AA_cff import Run2_2018_pp_on_AA
from Configuration.ProcessModifiers.run2_miniAOD_pp_on_AA_103X_cff import run2_miniAOD_pp_on_AA_103X
process = cms.Process('HiForest', Run2_2018_pp_on_AA,run2_miniAOD_pp_on_AA_103X)

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 125X, data")

###############################################################################

# input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/work/g/ginnocen/public/gtoccbarHI/MINIAOD_samples/PbPb18_MiniAODv1-v2_2550000_ea603a5c-2460-437d-b788-3ccbc332bd67.root'
        #'file:/data/gtoccbar/MINIAOD_samples/0032b2a4-ca1e-4df5-914c-38c69f3602f4_MC.root'
    ),
)

# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )

###############################################################################

# load Global Tag, geometry, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data_promptlike_hi', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

# This is the default centrality tag built-in in the miniAOD forest.
# The tag is given here for reference. Changing it has no effect on your analysis.
centralityTag = "CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1033p1x01_offline"
process.HiForestInfo.info.append(centralityTag)

print('\n')
print('\033[31m~*~ CENTRALITY TABLE FOR 2018 PBPB DATA ~*~\033[0m')
print('\033[36m~*~ TAG: ' + centralityTag + ' ~*~\033[0m')
print('\n')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

process.GlobalTag.toGet.extend([
    cms.PSet(
        record = cms.string("BTagTrackProbability3DRcd"),
        tag = cms.string("JPcalib_Data103X_2018PbPb_v1"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        )
    ])

###############################################################################

# root output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("HiForestMiniAOD.root"))

# # edm output for debugging purposes
# process.output = cms.OutputModule(
#     "PoolOutputModule",
#     fileName = cms.untracked.string('HiForestEDM.root'),
#     outputCommands = cms.untracked.vstring(
#         'keep *',
#         )
#     )

# process.output_path = cms.EndPath(process.output)

###############################################################################

# event analysis
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data
process.hltobject.triggerNames = trigger_list_data

process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')

process.particleFlowAnalyser.ptMin = 0
process.particleFlowAnalyser.absEtaMax = 7

################################
# electrons, photons, muons
SSHIRun2018A = "HeavyIonsAnalysis/EGMAnalysis/data/SSHIRun2018A.dat"
process.load('HeavyIonsAnalysis.EGMAnalysis.correctedElectronProducer_cfi')
process.correctedElectrons.correctionFile = SSHIRun2018A

process.load('HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi')
#process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.hltMuTree_cfi")
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doMuons = cms.bool(False)
process.ggHiNtuplizer.doElectrons = cms.bool(True)
process.ggHiNtuplizer.electronSrc = "correctedElectrons"
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
################################
# jet reco sequence
process.load('HeavyIonsAnalysis.JetAnalysis.akCs4PFJetSequence_pponPbPb_data_cff')
################################
# tracks
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")
###############################################################################

# ZDC RecHit Producer
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018Producer_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018RecHit_cfi')

process.load('HeavyIonsAnalysis.ZDCAnalysis.zdcanalyzer_cfi')
process.zdcanalyzer.doZDCRecHit = True
process.zdcanalyzer.doZDCDigi = False
process.zdcanalyzer.zdcRecHitSrc = cms.InputTag("QWzdcreco")
process.zdcanalyzer.calZDCDigi = True
################################


process.Trigger = cms.EDFilter( "TriggerResultsFilter",
      triggerConditions = cms.vstring(
         #'HLT_HIEle*',
         #'HLT_HIDoubleEle*',
         'HLT_HIL3Mu12_*',
         'HLT_HIL3Mu15_*',
         'HLT_HIL3Mu20_*',
         'HLT_HIL3DoubleMu*',
         'HLT_HIL2DoubleMu*',
         'HLT_HIL1DoubleMu*'
         ),
      hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
      l1tResults = cms.InputTag( "gtStage2Digis" ),
      l1tIgnoreMask = cms.bool( False ),
      l1techIgnorePrescales = cms.bool( False ),
      daqPartitions = cms.uint32( 1 ),
      throw = cms.bool( True )
      )






###############################################################################
# main forest sequence
process.forest = cms.Path(
    process.Trigger +
    process.HiForestInfo +
    process.hiEvtAnalyzer +
    process.hltanalysis +
    #process.hltobject +
    #process.l1object +
    process.trackSequencePbPb +
    process.particleFlowAnalyser +
    process.correctedElectrons +
    process.ggHiNtuplizer +
    # process.zdcdigi +
    # process.QWzdcreco +
    process.unpackedMuons +
    process.hltMuTree
    # process.zdcanalyzer #+
    #process.muonAnalyzer
    )

#customisation

addR3Jets = True
addR3FlowJets = False
addR4Jets = True
addR4FlowJets = False

# this is only for non-reclustered jets
addCandidateTagging = False


if addR3Jets or addR3FlowJets or addR4Jets or addR4FlowJets :
    process.load("HeavyIonsAnalysis.JetAnalysis.extraJets_cff")
    from HeavyIonsAnalysis.JetAnalysis.clusterJetsFromMiniAOD_cff import setupHeavyIonJets
    process.load("HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff")

    if addR3Jets :
        process.jetsR3 = cms.Sequence()
        setupHeavyIonJets('akCs3PF', process.jetsR3, process, isMC = 0, radius = 0.30, JECTag = 'AK3PF', doFlow = False)
        process.akCs3PFpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akCs3PFJetAnalyzer = process.akCs4PFJetAnalyzer.clone(jetTag = "akCs3PFpatJets", jetName = 'akCs3PF')
        process.forest += process.extraJetsData * process.jetsR3 * process.akCs3PFJetAnalyzer

    if addR3FlowJets :
        process.jetsR3flow = cms.Sequence()
        setupHeavyIonJets('akCs3PFFlow', process.jetsR3flow, process, isMC = 0, radius = 0.30, JECTag = 'AK3PF', doFlow = True)
        process.akCs3PFFlowpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akFlowPuCs3PFJetAnalyzer = process.akCs4PFJetAnalyzer.clone(jetTag = "akCs3PFFlowpatJets", jetName = 'akCs3PFFlow')
        process.forest += process.extraFlowJetsData * process.jetsR3flow * process.akFlowPuCs3PFJetAnalyzer

    if addR4Jets :
        # Recluster using an alias "0" in order not to get mixed up with the default AK4 collections
        process.jetsR4 = cms.Sequence()
        setupHeavyIonJets('akCs0PF', process.jetsR4, process, isMC = 0, radius = 0.40, JECTag = 'AK4PF', doFlow = False)
        process.akCs0PFpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akCs4PFJetAnalyzer.jetTag = 'akCs0PFpatJets'
        process.akCs4PFJetAnalyzer.jetName = 'akCs0PF'
        process.forest += process.extraJetsData * process.jetsR4 * process.akCs4PFJetAnalyzer

    if addR4FlowJets :
        process.jetsR4flow = cms.Sequence()
        setupHeavyIonJets('akCs4PFFlow', process.jetsR4flow, process, isMC = 0, radius = 0.40, JECTag = 'AK4PF', doFlow = True)
        process.akCs4PFFlowpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akFlowPuCs4PFJetAnalyzer.jetTag = 'akCs4PFFlowpatJets'
        process.akFlowPuCs4PFJetAnalyzer.jetName = 'akCs4PFFlow'
        process.forest += process.extraFlowJetsData * process.jetsR4flow * process.akFlowPuCs4PFJetAnalyzer

    process.hiPuRhoAnalyzer = cms.EDAnalyzer('HiFJRhoAnalyzer',
        etaMap = cms.InputTag('hiPuRho','mapEtaEdges','HiForest'),
        rho = cms.InputTag('hiPuRho','mapToRho'),
        rhoExtra = cms.InputTag('hiPuRho','mapToRhoExtra'),
        rhom = cms.InputTag('hiPuRho','mapToRhoM'),
        rhoCorr = cms.InputTag('hiPuRho','mapToRhoMedian'),
        rhomCorr = cms.InputTag('hiPuRho','mapToRhoM'),
        rhoCorr1Bin = cms.InputTag('hiPuRho','mapToRho'),
        rhomCorr1Bin = cms.InputTag('hiPuRho','mapToRhoM'),
        nTow = cms.InputTag('hiPuRho','mapToNTow'),
        towExcludePt = cms.InputTag('hiPuRho','mapToTowExcludePt'),
        towExcludePhi = cms.InputTag('hiPuRho','mapToTowExcludePhi'),
        towExcludeEta = cms.InputTag('hiPuRho','mapToTowExcludeEta'),
        rhoGrid = cms.InputTag('hiFJGridEmptyAreaCalculator','mapRhoVsEtaGrid'),
        meanRhoGrid = cms.InputTag('hiFJGridEmptyAreaCalculator','mapMeanRhoVsEtaGrid'),
        etaMaxRhoGrid = cms.InputTag('hiFJGridEmptyAreaCalculator','mapEtaMaxGrid'),
        etaMinRhoGrid = cms.InputTag('hiFJGridEmptyAreaCalculator','mapEtaMinGrid'),
        rhoFlowFitParams = cms.InputTag('hiFJRhoFlowModulationProducer','rhoFlowFitParams'),
        ptJets = cms.InputTag('hiPuRho', 'ptJets'),
        etaJets = cms.InputTag('hiPuRho', 'etaJets'),
        areaJets = cms.InputTag('hiPuRho', 'areaJets'),
        useModulatedRho = cms.bool(False),
        )
    #process.forest += process.hiPuRhoAnalyzer


if addCandidateTagging:
    process.load("HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff")

    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
    updateJetCollection(
        process,
        jetSource = cms.InputTag('slimmedJets'),
        jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
        btagDiscriminators = ['pfCombinedSecondaryVertexV2BJetTags', 'pfDeepCSVDiscriminatorsJetTags:BvsAll', 'pfDeepCSVDiscriminatorsJetTags:CvsB', 'pfDeepCSVDiscriminatorsJetTags:CvsL'], ## to add discriminators,
        btagPrefix = 'TEST',
    )

    process.updatedPatJets.addJetCorrFactors = False
    process.updatedPatJets.discriminatorSources = cms.VInputTag(
        cms.InputTag('pfDeepCSVJetTags:probb'),
        cms.InputTag('pfDeepCSVJetTags:probc'),
        cms.InputTag('pfDeepCSVJetTags:probudsg'),
        cms.InputTag('pfDeepCSVJetTags:probbb'),
    )

    process.akCs4PFJetAnalyzer.jetTag = "updatedPatJets"

    process.forest.insert(1,process.candidateBtagging*process.updatedPatJets)


#########################
# Event Selection -> add the needed filters here
#########################

process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.load('HeavyIonsAnalysis.EventAnalysis.hffilter_cfi')
process.pphfCoincFilter2Th4 = cms.Path(process.phfCoincFilter2Th4)
process.pAna = cms.EndPath(process.skimanalysis)
