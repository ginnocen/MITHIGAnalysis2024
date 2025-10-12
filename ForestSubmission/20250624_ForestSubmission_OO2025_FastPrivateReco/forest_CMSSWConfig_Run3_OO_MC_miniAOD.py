### HiForest CMSSW Configuration
# Collisions: Oxygen-Oxygen
# Input: miniAOD
# Type: mc

import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_2025_OXY_cff import Run3_2025_OXY
process = cms.Process('HiForest', Run3_2025_OXY)

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 150X, mc")

###############################################################################

# input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/user/srdas/Hijing_HeO_5362GeV_NoPU_100kEvents/250603_Hijing_HeO_5362GeV_NoPU_100kEvents_ppReco_00/250604_045759/0000/step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_11.root'
    ),
    secondaryFileNames = cms.untracked.vstring(
        '/store/user/srdas/Hijing_HeO_5362GeV_NoPU_100kEvents/250603_Hijing_HeO_5362GeV_NoPU_100kEvents_DigiRaw_01/250604_031449/0000/step2_DIGI_L1_DIGI2RAW_HLT_20.root'
    ),
)

# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

###############################################################################

# load Global Tag, geometry, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '150X_mcRun3_2025_forOO_realistic_v1', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
             tag = cms.string("JPcalib_MC103X_2018PbPb_v4"),
             connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
         )
])

# Define centrality binning
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")

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

#############################
# Gen Analyzer
#############################
process.load('HeavyIonsAnalysis.EventAnalysis.HiGenAnalyzer_cfi')

# event analysis
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
process.particleFlowAnalyser.ptMin = cms.double(0.)
process.particleFlowAnalyser.absEtaMax = cms.double(6.)
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_mc_cfi')
process.hiEvtAnalyzer.doHFfilters = cms.bool(False)
process.hiEvtAnalyzer.doMC = cms.bool(True) # general MC info
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')
# process.hiEvtAnalyzer.doCentrality = cms.bool(False) # used for UPC

# add L1 MET filter
process.load('L1Trigger.L1TNtuples.l1MetFilterRecoTree_cfi')

#from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_mc
#process.hltobject.triggerNames = trigger_list_mc

################################
# electrons, photons, muons
#process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
#process.ggHiNtuplizer.doGenParticles = cms.bool(True)
#process.ggHiNtuplizer.doMuons = cms.bool(False)
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
################################
# jet reco sequence
process.load('HeavyIonsAnalysis.JetAnalysis.akCs4PFJetSequence_pponPbPb_mc_cff')
################################
# tracks
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")
#muons
process.load("HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")
process.muonAnalyzer.doGen = cms.bool(True)
###############################################################################

#########################
# ZDC RecHit Producer && Analyzer
#########################
# to prevent crash related to HcalSeverityLevelComputerRcd record
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")
process.load('HeavyIonsAnalysis.ZDCAnalysis.ZDCAnalyzersPbPb_cff')

###############################################################################
# main forest sequence
process.forest = cms.Path(
    process.HiForestInfo +
    process.centralityBin +
    process.hltanalysis +
#    process.hltobject +
    process.l1object +
    process.l1MetFilterRecoTree +
    process.trackSequencePP +
    process.particleFlowAnalyser +
    process.hiEvtAnalyzer +
    process.HiGenParticleAna
#    process.ggHiNtuplizer +
#    process.zdcSequencePbPb +
#    process.unpackedMuons +
#    process.muonAnalyzer
    )

#########################
# Event Selection -> add the needed filters here
#########################

process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.load('HeavyIonsAnalysis.EventAnalysis.hffilterPF_cfi')
process.pAna = cms.EndPath(process.skimanalysis)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    # digiLabel = cms.untracked.InputTag("hcalDigis"),
    digiLabel = cms.untracked.InputTag("simHcalUnsuppressedDigis","HFQIE10DigiCollection"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False) # only turn this on when you have or know how to produce "towerMaker"
)
# process.hfadc = cms.Path(process.HFAdcana)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('analysis')
ivars.outputFile = 'HiForestMiniAOD.root'
ivars.maxEvents = -1 
ivars.parseArguments() # get and parse the command line arguments
# process.source.fileNames = ivars.inputFiles
process.maxEvents.input = cms.untracked.int32(ivars.maxEvents)
process.TFileService.fileName = ivars.outputFile # keep for condor, remove for crab

