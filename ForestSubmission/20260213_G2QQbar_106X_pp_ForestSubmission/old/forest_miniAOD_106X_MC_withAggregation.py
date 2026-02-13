### HiForest Configuration
# Input: miniAOD
# Type: mc
import sys

import FWCore.ParameterSet.Config as cms
process = cms.Process('HiForest')
process.options = cms.untracked.PSet()

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 106X, mc")

# input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/himc/RunIISummer20UL17pp5TeVMiniAODv2/QCD_pThat-15_bJet_TuneCP5_5p02TeV-pythia8/MINIAODSIM/106X_mc2017_realistic_forppRef5TeV_v3-v3/2530000/67404659-5156-7549-AB1C-20D66F6142B1.root'
        #'/store/user/jmijusko/67404659-5156-7549-AB1C-20D66F6142B1.root'
        ),
    )


# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
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

process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
             tag = cms.string("JPcalib_MC94X_2017pp_v2"), # mc tag
            # tag = cms.string("JPcalib_Data4MCSwap_94X_2017pp_v3"), # data tag for swap 
            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
         )
      ])

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
# process.hltobject.triggerNames = trigger_list_mc
# process.hltobject.triggerNames = cms.vstring('HLT_HIAK4CaloJet30_v', 'HLT_HIAK4CaloJet40_v', 'HLT_HIAK4CaloJet60_v')

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
'''
# tracks
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")
#muons
process.load("HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")
process.muonAnalyzer.doGen = cms.bool(True)
'''
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
    process.HiForestInfo +
    process.hltanalysis +
    process.hiEvtAnalyzer +
    #process.tagInfoSequence +
    process.genJetSequence #+ 
    #process.recoJetSequence + 
    #process.rhoSequence 
)

## Customization
doDeclustering = True
doAggregation = True
doChargedOnly = True
doLatekt_ = False

matchJets = True
jetPtMin = 15
jetAbsEtaMax = 2.5
doBtagging  =  True  
isMC = True

tmva_variables = ["trkIp3dSig", "trkIp2dSig", "trkDistToAxis",
                  "svtxdls", "svtxdls2d", "svtxm", "svtxmcorr",
                  "svtxnormchi2", "svtxNtrk", "svtxTrkPtOverSv",
                  "jtpt"]

jetLabels = ["2"]

#common gen stuff to all cone sizes
if doDeclustering:
    process.load("GeneratorInterface.RivetInterface.mergedGenParticles_cfi")
    process.genJetSequence += process.mergedGenParticles
    ## Produces a reco::GenParticleCollection named mergedGenParticles

    process.load("RecoHI.HiJetAlgos.HFdecayProductTagger_cfi")
    process.HFdecayProductTagger.genParticles = cms.InputTag("mergedGenParticles")
    process.HFdecayProductTagger.tagBorC = cms.bool(True) # tag B
    process.genJetSequence += process.HFdecayProductTagger
    taggedGenParticlesName_ = "HFdecayProductTagger"
    ## Produces a std::vector<pat::PackedGenParticle> named HFdecayProductTagger

    process.bDecayAna = process.HiGenParticleAna.clone(
        genParticleSrc = cms.InputTag(taggedGenParticlesName_, "patPackedGenParticles"),
        useRefVector = cms.untracked.bool(False),
        partonMEOnly = cms.untracked.bool(False),
        chargedOnly = doChargedOnly,
        doHI = False,
        etaMax = cms.untracked.double(10),
        ptMin = cms.untracked.double(0),
        stableOnly = False
    )
    #process.genJetSequence += process.bDecayAna
    ## Creates the gen particle ntuple bDecayAna/hi




# add candidate tagging for all selected jet radii                                                                                                                                                                  
from HeavyIonsAnalysis.JetAnalysis.setupJets_ppRef_cff import candidateBtaggingMiniAOD

for jetLabel in jetLabels:

    jetR = 0.1*float(jetLabel)
    if jetLabel == "0": jetR = 0.4

    candidateBtaggingMiniAOD(process, isMC = True, jetPtMin = jetPtMin, jetCorrLevels = ['L2Relative', 'L3Absolute'], doBtagging = doBtagging, labelR = jetLabel, doAggregation = doAggregation)


    # setup jet analyzer                                                                                                                                                                                                
    setattr(process,"ak"+jetLabel+"PFJetAnalyzer",process.ak4PFJetAnalyzer.clone())
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetTag = "patJetsAK"+jetLabel+"PFCHSAggr"
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetName = 'ak'+jetLabel+'PF'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchJets = matchJets
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").matchTag = 'patJetsAK'+jetLabel+'PFUnsubJets'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").originalTag = 'selectedUpdatedPatJetsAK'+jetLabel+'PFCHSBtag'
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetPtMin = jetPtMin
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetAbsEtaMax = cms.untracked.double(jetAbsEtaMax)
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rParam = 0.4 if jetLabel=="0" else float(jetLabel)*0.1
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").jetFlavourInfos = cms.InputTag("patJetFlavourAssociationAK"+jetLabel+"PFCHSAggr")
    if jetLabel != "0": getattr(process,"ak"+jetLabel+"PFJetAnalyzer").genjetTag = "ak"+jetLabel+"GenJetsReclusterNoNu"
    
    
    ## NOTE:  This is not yet set up to run multiple cone sizes in the same pass!!


    # cone size dependent but not dependent on declustering
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").rhoSrc = cms.InputTag("fixedGridRhoFastjetAll")
    getattr(process,"ak"+jetLabel+"PFJetAnalyzer").doTracks = False 
    #process.forest += getattr(process,"recoJetSequence")
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
    


#process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
#process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
#process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
#process.pAna = cms.EndPath(process.skimanalysis)
