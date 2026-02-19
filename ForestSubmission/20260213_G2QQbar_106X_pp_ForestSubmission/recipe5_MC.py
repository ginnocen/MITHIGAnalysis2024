### HiForest Configuration
# Input: miniAOD
# Type: mc
import sys

import FWCore.ParameterSet.Config as cms
process = cms.Process('HiForest')
process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('StdException') 
)

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 106X, mc")

# input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/himc/RunIISummer20UL17pp5TeVMiniAODv2/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/MINIAODSIM/106X_mc2017_realistic_forppRef5TeV_v3-v3/40000/20A6EC56-8E3E-004F-9ED0-6949CD64B93D.root'
        ),
    )


# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# load Global Tag, geometry, etc.
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_mc2017_realistic_v10', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

'''
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
             tag = cms.string("JPcalib_MC94X_2017pp_v2"), # mc tag
            # tag = cms.string("JPcalib_Data4MCSwap_94X_2017pp_v3"), # data tag for swap 
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
         )
      ])
'''
###############################################################################

# root output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("HiForestMiniAOD.root"))

'''
# edm output for debugging purposes
process.output = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('HiForestEDM.root'),
    outputCommands = cms.untracked.vstring(
        'keep *',
    )
)
process.output_path = cms.EndPath(process.output)
'''

###############################################################################

#############################
# Gen Analyzer
#############################
process.load('HeavyIonsAnalysis.EventAnalysis.HiGenAnalyzer_cfi')
# making cuts looser so that we can actually check dNdEta
process.HiGenParticleAna.ptMin = cms.untracked.double(0.4) # default is 5
process.HiGenParticleAna.etaMax = cms.untracked.double(5.) # default is 2.5

# event analysis
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_mc_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_mc
process.hltobject.triggerNames = trigger_list_mc


'''
################################
# electrons, photons, muons
SS2018PbPbMC = "HeavyIonsAnalysis/EGMAnalysis/data/SS2018PbPbMC.dat"
process.load('HeavyIonsAnalysis.EGMAnalysis.correctedElectronProducer_cfi')
process.correctedElectrons.correctionFile = SS2018PbPbMC
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doGenParticles = cms.bool(True)
process.ggHiNtuplizer.doMuons = cms.bool(False)
process.ggHiNtuplizer.electronSrc = "correctedElectrons"
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
'''
################################
# jet reco sequence
process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_pponPbPb_mc_cff')
process.tagInfoSequence = cms.Sequence()
process.genJetSequence = cms.Sequence()
process.recoJetSequence = cms.Sequence()
################################

# tracks
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")
# Fix lostTracks issue for ppTracks - use empty collection since it doesn't exist in miniAOD
process.ppTracks.lostTracksSrc = cms.InputTag("packedPFCandidates")

#muons
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi")
process.unpackedMuons.muonSelectors = cms.vstring()
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")
process.muonAnalyzer.doGen = cms.bool(True)

#################################
# rho
process.load("RecoJets.JetProducers.fixedGridRhoProducerFastjet_cfi")
process.fixedGridRhoFastjetAll.pfCandidatesTag = cms.InputTag("packedPFCandidates")
process.rhoSequence = cms.Sequence(
    process.fixedGridRhoFastjetAll #process.hiFJGridEmptyAreaCalculator + process.hiFJRhoProducer + process.hiFJRhoAnalyzer
)

###############################################################################

###############################################################################
# main forest sequence
process.forest = cms.Path(
    process.unpackedTracksAndVertices +  # Run first to skip entire path on exception
    process.HiForestInfo +
    process.hltanalysis +
    process.hiEvtAnalyzer +
    process.trackSequencePP +
    process.unpackedMuons +
    process.muonAnalyzer +
    #process.tagInfoSequence +
    process.genJetSequence +
    process.recoJetSequence + 
    process.rhoSequence
)



#################### D finder #################
runOnMC = True # or True, up to if it's data or MC
VtxLabel = "offlineSlimmedPrimaryVertices"
TrkLabel = "packedPFCandidates"
TrkChi2Label = "packedPFCandidateTrackChi2"
GenLabel = "prunedGenParticles"
from Bfinder.finderMaker.finderMaker_75X_cff import finderMaker_75X,setCutForAllChannelsDfinder
finderMaker_75X(process, runOnMC, VtxLabel, TrkLabel, TrkChi2Label, GenLabel)
# Fix muonMatch to use miniAOD collections
process.muonMatch.src = cms.InputTag("slimmedMuons")
process.muonMatch.matched = cms.InputTag("prunedGenParticles")
process.Dfinder.tkPtCut = cms.double(1.) # before fit
process.Dfinder.tkEtaCut = cms.double(2.4) # before fit
process.Dfinder.Dchannel = cms.vint32(1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0)
setCutForAllChannelsDfinder(process, dPtCut = 3, VtxChiProbCut = 0.05, svpvDistanceCut = 2.5, alphaCut = 999.)
# process.Dfinder.printInfo = cms.bool(False)
process.dfinder = cms.Path(process.unpackedTracksAndVertices + process.DfinderSequence)

#################### B finder #################
process.Bfinder.tkPtCut = cms.double(1.) # before fit
process.Bfinder.tkEtaCut = cms.double(2.4) # before fit
process.Bfinder.jpsiPtCut = cms.double(0.0) # before fit
process.Bfinder.Bchannel = cms.vint32(1, 1, 0, 0, 0, 0, 0)
process.Bfinder.bPtCut = cms.vdouble(3.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0) # before fit
process.Bfinder.VtxChiProbCut = cms.vdouble(0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05)
process.Bfinder.svpvDistanceCut = cms.vdouble(3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 0.0)
process.Bfinder.MuonTriggerMatchingPath = cms.vstring("")
process.Bfinder.MuonTriggerMatchingFilter = cms.vstring("")
process.BfinderSequence.insert(0, process.unpackedMuons)
process.BfinderSequence.insert(0, process.unpackedTracksAndVertices)
process.bfinder = cms.Path(process.unpackedTracksAndVertices + process.BfinderSequence)




## Customization 
doAggregation = False   
doChargedOnly = True
doLatekt_ = False

tmva_variables = ["trkIp3dSig", "trkIp2dSig", "trkDistToAxis",
                  "svtxdls", "svtxdls2d", "svtxm", "svtxmcorr",
                  "svtxnormchi2", "svtxNtrk", "svtxTrkPtOverSv",
                  "jtpt"]


matchJets = False  
jetPtMin = 45
jetAbsEtaMax = 2.5
doBtagging = True
isMC = True

doSvtx = True
doTracks = True


jetLabels = ["3"]

#common gen stuff to all cone sizes

process.load("GeneratorInterface.RivetInterface.mergedGenParticles_cfi")
process.genJetSequence += process.mergedGenParticles
## Produces a reco::GenParticleCollection named mergedGenParticles

process.load("RecoHI.HiJetAlgos.HFdecayProductTagger_cfi")
process.HFdecayProductTagger.genParticles = cms.InputTag("mergedGenParticles")
process.HFdecayProductTagger.tagBorC = cms.bool(True) # tag B
process.genJetSequence += process.HFdecayProductTagger
taggedGenParticlesName_ = "HFdecayProductTagger"
## Produces a std::vector<pat::PackedGenParticle> named HFdecayProductTagger

# Fix for jet flavor association - create hadron and parton selectors
from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
    particles = "prunedGenParticles"
)
process.genJetSequence += process.selectedHadronsAndPartons

# Create the patJetPartons for jet flavor clustering
from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import patJetPartons
process.patJetPartons = patJetPartons.clone(
    particles = "prunedGenParticles"
)
process.genJetSequence += process.patJetPartons

# add candidate tagging for all selected jet radii                                                                                                                                                                  
from HeavyIonsAnalysis.JetAnalysis.setupJets_ppRef_cff import candidateBtaggingMiniAOD

for jetLabel in jetLabels:

    jetR = 0.1*float(jetLabel)
    if jetLabel == "0": jetR = 0.4

    candidateBtaggingMiniAOD(process, isMC = True, jetPtMin = jetPtMin, jetCorrLevels = ['L2Relative', 'L3Absolute'], doBtagging = doBtagging, labelR = jetLabel, doAggregation = doAggregation)

        # setup jet analyzer                                                                                                                                                                                                
    setattr(process,"ak"+jetLabel+"PFJetAnalyzer",process.ak4PFJetAnalyzer.clone())
    #getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetTag = "patJetsAK"+jetLabel+"PFCHSAggr"
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetTag = "selectedUpdatedPatJetsAK"+jetLabel+"PFCHSBtag"
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetName = 'ak'+jetLabel+'PF'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchJets = matchJets
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchTag = 'patJetsAK'+jetLabel+'PFUnsubJets'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").originalTag = 'selectedUpdatedPatJetsAK'+jetLabel+'PFCHSBtag'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetPtMin = jetPtMin
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetAbsEtaMax = cms.untracked.double(jetAbsEtaMax)
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rParam = 0.4 if jetLabel=="0" else float(jetLabel)*0.1
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetFlavourInfos = cms.InputTag("patJetFlavourAssociationAK"+jetLabel+"PFCHSAggr")
    if jetLabel != "0": getattr(process,"ak"+jetLabel+"PFJetAnalyzer").genjetTag = "ak"+jetLabel+"GenJetsReclusterNoNu"

    
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").genParticles = cms.untracked.InputTag(taggedGenParticlesName_, "patPackedGenParticles")
        
    process.load("RecoHI.HiJetAlgos.TrackToGenParticleMapProducer_cfi")
    getattr(process,"TrackToGenParticleMapProducer").jetSrc = "selectedUpdatedPatJetsAK"+jetLabel+"PFCHSBtag"
    process.TrackToGenParticleMapProducer.genParticleSrc = cms.InputTag(taggedGenParticlesName_, "patPackedGenParticles")
    process.TrackToGenParticleMapProducer.chargedOnly = doChargedOnly
    process.genJetSequence += process.TrackToGenParticleMapProducer
    ## Creates the genConstitToGenParticleMap and trackToGenParticleMap

    process.load("RecoHI.HiJetAlgos.dynGroomedPATJets_cfi")
    process.dynGroomedGenJets = process.dynGroomedPATJets.clone(
        chargedOnly = cms.bool(doChargedOnly),
        aggregateHF = cms.bool(doAggregation),
        # aggregateHF = cms.bool(True),
        jetSrc = cms.InputTag("selectedUpdatedPatJetsAK"+jetLabel+"PFCHSBtag"),
        constitSrc = cms.InputTag("packedGenParticles"),
        doGenJets = cms.bool(True),
        candToGenParticleMap = cms.InputTag("TrackToGenParticleMapProducer", "genConstitToGenParticleMap"),
        doLateKt = cms.bool(doLatekt_),
        rParam = float(jetLabel)*0.1
    )
    process.genJetSequence += process.dynGroomedGenJets
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").groomedGenJets = cms.untracked.InputTag("dynGroomedGenJets")
    ## Creates the gen jet subjets
    
    process.dynGroomedPFJets = process.dynGroomedPATJets.clone(
        chargedOnly = cms.bool(doChargedOnly),
        aggregateHF = cms.bool(doAggregation),
        # aggregateHF = cms.bool(False),
        jetSrc = cms.InputTag("selectedUpdatedPatJetsAK"+jetLabel+"PFCHSBtag"),
        constitSrc = cms.InputTag("packedPFCandidates"),
        doGenJets = cms.bool(False),
        candToGenParticleMap = cms.InputTag("TrackToGenParticleMapProducer", "trackToGenParticleMap"),
        aggregateWithTruthInfo = cms.bool(False),
        aggregateWithXGB = cms.bool(False),
        aggregateWithTMVA = cms.bool(True),
        aggregateWithCuts = cms.bool(False),
        #xgb_path = cms.FileInPath("RecoHI/HiJetAlgos/data/sig_vs_bkg.model"),
        tmva_path = cms.FileInPath("RecoHI/HiJetAlgos/data/TMVAClassification_BDTG.weights.xml"),
        tmva_variables = cms.vstring(tmva_variables),
        doLateKt = cms.bool(doLatekt_),
        trkInefRate = cms.double(0.),
        rParam = float(jetLabel)*0.1
    )
    process.dynGroomedPFJets.ipTagInfoLabel = "pfImpactParameter"
    process.dynGroomedPFJets.svTagInfoLabel = "pfInclusiveSecondaryVertexFinder"
    process.recoJetSequence += process.dynGroomedPFJets
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").groomedJets = cms.untracked.InputTag("dynGroomedPFJets")
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doSubJetsNew = cms.untracked.bool(True)

    ## NOTE:  This is not yet set up to run multiple cone sizes in the same pass!!


    # cone size dependent but not dependent on declustering
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rhoSrc = cms.InputTag("fixedGridRhoFastjetAll")
    #process.forest += getattr(process,"recoJetSequence")

    if doTracks:
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doTracks = cms.untracked.bool(True)
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").ipTagInfoLabel = cms.untracked.string("pfImpactParameter")
    if doSvtx:
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doSvtx = cms.untracked.bool(True)
        getattr(process,"ak"+jetLabel+"PFJetAnalyzer").svTagInfoLabel = cms.untracked.string("pfInclusiveSecondaryVertexFinder")
    process.forest += getattr(process,"ak"+jetLabel+"PFJetAnalyzer")



#########################
# Jet Selection
#########################
    
# for b tagging SF
# process.mujetSelector = cms.EDFilter("PatJetXSelector",
#                              src = cms.InputTag("slimmedJets"),
#                              offPV = cms.InputTag("offlineSlimmedPrimaryVertices"),
#                              cut = cms.string("pt > 5.0 && abs(rapidity()) < 3."),
#                              dummy = cms.bool(False)
#                          )
# process.recoJetSequence += process.mujetSelector
# process.ak2PFJetAnalyzer.mujetTag = cms.InputTag("mujetSelector")

#########################
# Event Selection -> add the needed filters here
#########################
    
#process.patJetsAK3PFUnsubJets.addBTagInfo = True
#process.patJetsAK3PFUnsubJets.addTagInfos = True
#process.patJetsAK3PFUnsubJets.tagInfoSources = cms.VInputTag(["pfInclusiveSecondaryVertexFinderTagInfos","pfImpactParameterTagInfos"])
#process.patJetsAK3PFUnsubJets.addDiscriminators = False



process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
#process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.primaryVertexFilter.filter = cms.bool(False)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)

# For miniAOD: convert packedPFCandidates to track collection for beam scraping filter
#process.load("RecoTracker.FinalTrackSelectors.unpackedTracksAndVertices_cfi")


process.NoScraping = cms.EDFilter("FilterOutScraping",
 applyfilter = cms.untracked.bool(False),
 debugOn = cms.untracked.bool(False),
 numtrack = cms.untracked.uint32(10),
 thresh = cms.untracked.double(0.25),
 src = cms.untracked.InputTag("unpackedTracksAndVertices"),
 filter = cms.bool(False)
)

process.pBeamScrapingFilter=cms.Path(process.unpackedTracksAndVertices + process.NoScraping)

process.pAna = cms.EndPath(process.skimanalysis)








