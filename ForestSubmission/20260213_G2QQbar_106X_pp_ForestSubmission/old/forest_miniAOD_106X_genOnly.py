### HiForest Configuration
# Input: miniAOD
# Type: mc
import sys

# fname = sys.argv[2]
# print("file in:", fname)

import FWCore.ParameterSet.Config as cms
process = cms.Process('HiForest')
process.options = cms.untracked.PSet()

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 106X, mc")

# import subprocess, os
# version = subprocess.check_output(
#     ['git', '-C', os.path.expandvars('$CMSSW_BASE/src'), 'describe', '--tags'])
# if version == '':
#     version = 'no git info'
# process.HiForestInfo.HiForestVersion = cms.string(version)

###############################################################################

# input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/user/mnguyen/Pythia8_inclusiveJets_FSRup/Pythia8_inclusiveJets_FSRup/240701_213456/0000/HIN-RunIISummer20UL17pp5TeVGS-00003-fragment_py_GEN_1.root'
        ),
    )

# Select specific event
# process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('1:7897')
# process.source.eventsToProcess = cms.untracked.VEventRange('1:110394257')

# number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# Multi-thread 
# process.options.numberOfThreads = cms.untracked.int32(8)

# To skip events with 'Found zero products matching all criteria' error
# process.options = cms.untracked.PSet(SkipEvent = cms.untracked.vstring('ProductNotFound'))

###############################################################################

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

# # edm output for debugging purposes
# process.output = cms.OutputModule(
#     "PoolOutputModule",
#     fileName = cms.untracked.string('HiForestEDM.root'),
#     outputCommands = cms.untracked.vstring(
#         'keep *',
#         )
# )
# process.output_path = cms.EndPath(process.output)

###############################################################################

#############################
# Gen particle Analyzer
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

################################
# jets
process.load('HeavyIonsAnalysis.JetAnalysis.inclusiveGenJetAnalyzer_cff')
process.ak4PFJetAnalyzer = process.inclusiveGenJetAnalyzer.clone()
process.genJetSequence = cms.Sequence()
################################


###############################################################################
# main forest sequence
process.forest = cms.Path(
    process.HiForestInfo 
    + process.hiEvtAnalyzer
    + process.genJetSequence
    + process.ak4PFJetAnalyzer
    )

## Customization
process.hiEvtAnalyzer.doHiMC = False
process.hiEvtAnalyzer.doVertex = False

## Number of b and c hadrons
from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
    particles = cms.InputTag("genParticles")
)  
from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.jetFlavourInfosAK4PFJets = ak4JetFlavourInfos.clone(
    jets = cms.InputTag("ak4GenJetsNoNu")
)
process.genJetSequence.insert(0, process.selectedHadronsAndPartons*process.jetFlavourInfosAK4PFJets)
process.ak4PFJetAnalyzer.jetFlavourInfos = cms.InputTag("jetFlavourInfosAK4PFJets")

process.ak4PFJetAnalyzer.genjetTag = cms.InputTag("ak4GenJetsNoNu")
process.ak4PFJetAnalyzer.doSubJetsNew = cms.untracked.bool(True)


doDeclustering = True
doAggregation = True
doChargedOnly = True
doLatekt_ = False

if doDeclustering:
    process.load("RecoHI.HiJetAlgos.HFdecayProductTagger_cfi")
    process.HFdecayProductTagger.genParticles = cms.InputTag("genParticles")
    process.HFdecayProductTagger.tagBorC = cms.bool(True) # tag B
    process.genJetSequence += process.HFdecayProductTagger
    ## Produces a std::vector<pat::PackedGenParticle> named HFdecayProductTagger
    ## Produces a std::vector<reco::GenParticle> named HFdecayProductTagger, recoGenParticles

    process.ak4PFJetAnalyzer.genParticles = cms.InputTag("HFdecayProductTagger", "patPackedGenParticles")

    process.bDecayAna = process.HiGenParticleAna.clone(
        genParticleSrc = cms.InputTag("HFdecayProductTagger", "patPackedGenParticles"),
        useRefVector = cms.untracked.bool(False),
        partonMEOnly = cms.untracked.bool(False),
        chargedOnly = doChargedOnly,
        doHI = False,
        etaMax = cms.untracked.double(10),
        ptMin = cms.untracked.double(0),
        stableOnly = False
    )
    process.genJetSequence += process.bDecayAna
    ## Creates the gen particle ntuple bDecayAna/hi

    process.load("RecoHI.HiJetAlgos.dynGroomedGenJets_cfi")
    process.dynGroomedGenJets = process.dynGroomedGenJets.clone(
        chargedOnly = cms.bool(doChargedOnly),
        aggregateHF = cms.bool(doAggregation),
        genJetSrc = cms.InputTag("ak4GenJetsNoNu"),
        taggedGenParticleSrc = cms.InputTag("HFdecayProductTagger", "recoGenParticles"),
        doLateKt = cms.bool(doLatekt_),
    )
    process.genJetSequence += process.dynGroomedGenJets
    process.ak4PFJetAnalyzer.groomedGenJets = cms.untracked.InputTag("dynGroomedGenJets")
    ## Creates the gen jet subjets

#########################
# Event Selection -> add the needed filters here
#########################


#process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
#process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
#process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
#process.pAna = cms.EndPath(process.skimanalysis)
