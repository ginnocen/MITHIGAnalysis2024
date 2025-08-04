#include <algorithm>
#include <iostream>
#include <set>
using namespace std;

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "MCReweighting.h"
#include "TrackResidualCorrector.h"
#include "eventSelectionCorrection.h"
#include "tnp_weight.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"
#include "trackingEfficiency2024ppref.h"
#include "trackingEfficiency2025OO.h"

#include "include/cent_OO_hijing_PF.h"
#include "include/parseFSCandPPSInfo.h"
#include "include/skimSelectionBits_OO_PP.h"

bool logical_or_vectBool(std::vector<bool> *vec) {
  return std::any_of(vec->begin(), vec->end(), [](bool b) { return b; });
}

// Helper function to convert a string to lowercase
std::string toLower(const std::string &str) {
  std::string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
  return lowerStr;
}

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);
  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");
  bool IsData = CL.GetBool("IsData", false);
  string CollisionSystem = CL.Get("CollisionSystem", "OO");
  double Fraction = CL.GetDouble("Fraction", 1.00);
  int ApplyTriggerRejection =
      CL.GetInteger("ApplyTriggerRejection", 0); // trigger = 0 for no rejection, 1 for ZeroBias, 2 for MinBias
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  bool ApplyTrackRejection = CL.GetBool("ApplyTrackRejection", false);
  bool rejectTracksBelowPtEnabled = CL.GetBool("rejectTracksBelowPtEnabled", true); // if false, no rejection is applied
  float rejectTracksBelowPt = CL.GetDouble("rejectTracksBelowPt", 3.0);
  if (rejectTracksBelowPt < 0.4) {
    std::cout << "WARNING: rejectTracksBelowPt is set to " << rejectTracksBelowPt
              << ". This is lower than the default value of 0.4 GeV/c. "
                 "This may lead to unexpected results."
              << std::endl;
  }
  string CorrectionPath = CL.Get("CorrectionPath");
  int sampleType = CL.GetInteger(
      "sampleType", -1); // 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcrechit");
  string PPSTreeName = CL.Get("PPSTree", "ppsanalyzer/ppstracks");
  string FSCTreeName = CL.Get("FSCTree", "fscanalyzer/fscdigi");
  bool HideProgressBar = CL.GetBool("HideProgressBar", false);
  bool DebugMode = CL.GetBool("DebugMode", false);
  bool includeFSCandPPSMode = CL.GetBool("includeFSCandPPSMode", false);
  bool includeL1EMU = CL.GetBool("includeL1EMU", true);

  int saveTriggerBitsMode = 0;
  if (CollisionSystem != "pp" && CollisionSystem != "OO" && CollisionSystem != "pO" && CollisionSystem != "NeNe") {
    std::cout << "ERROR: Collision system must be pp, OO, pO or NeNe. Exiting." << std::endl;
    return 1;
  }
  if (CollisionSystem == "OO" || CollisionSystem == "NeNe") {
    saveTriggerBitsMode = 1;
  } // save trigger bits for OO and NeNe
  else if (CollisionSystem == "pO") {
    saveTriggerBitsMode = 2;
  } // save trigger bits for pO

  // TRACKING EFFICIENCY
  TrkEff2024ppref *TrackEfficiencyPP2024 = nullptr;
  TrkEff2024ppref *TrackEfficiencyPP2024_DCALoose = nullptr;
  TrkEff2024ppref *TrackEfficiencyPP2024_DCATight = nullptr;

  TrkEff2025OO *TrackEfficiencyOO2025 = nullptr;
  TrkEff2025OO *TrackEfficiencyOO2025_DCALoose = nullptr;
  TrkEff2025OO *TrackEfficiencyOO2025_DCATight = nullptr;

  TrkEff2025OO *TrackEfficiencyNeNe2025 = nullptr;
  TrkEff2025OO *TrackEfficiencyNeNe2025_DCALoose = nullptr;
  TrkEff2025OO *TrackEfficiencyNeNe2025_DCATight = nullptr;

  /*
  TrkEff2025pO *TrackEfficiencypO2025 = nullptr;
  TrkEff2025pO *TrackEfficiencyNeNe2025_DCALoose = nullptr;
  TrkEff2025pO *TrackEfficiencyNeNe2025_DCATight = nullptr;
  */

  if (CollisionSystem == "pp") {
    TrackEfficiencyPP2024 = new TrkEff2024ppref(
        true, Form("%s/Eff_ppref_2024_Pythia_minBias_NopU_2D_Nominal_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyPP2024_DCALoose = new TrkEff2024ppref(
        true, Form("%s/Eff_ppref_2024_Pythia_minBias_NopU_2D_Loose_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyPP2024_DCATight = new TrkEff2024ppref(
        true, Form("%s/Eff_ppref_2024_Pythia_minBias_NopU_2D_Tight_Official.root", CorrectionPath.c_str()));
  } else if (CollisionSystem == "OO") {
    TrackEfficiencyOO2025 =
        new TrkEff2025OO(true,
                         Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Nominal_Official.root",
                              CorrectionPath.c_str()),
                         Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Nominal_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyOO2025_DCALoose = new TrkEff2025OO(
        true,
        Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Loose_Official.root", CorrectionPath.c_str()),
        Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Loose_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyOO2025_DCATight = new TrkEff2025OO(
        true,
        Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Tight_Official.root", CorrectionPath.c_str()),
        Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Tight_Official.root", CorrectionPath.c_str()));
  } else if (CollisionSystem == "NeNe") {
    // FIXME: NeNe corrections are currently the same as OO
    TrackEfficiencyNeNe2025 =
        new TrkEff2025OO(true,
                         Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Nominal_Official.root",
                              CorrectionPath.c_str()),
                         Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Nominal_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyNeNe2025_DCALoose = new TrkEff2025OO(
        true,
        Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Loose_Official.root", CorrectionPath.c_str()),
        Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Loose_Official.root", CorrectionPath.c_str()));
    TrackEfficiencyNeNe2025_DCATight = new TrkEff2025OO(
        true,
        Form("%s/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Tight_Official.root", CorrectionPath.c_str()),
        Form("%s/Eff_OO_2025_Hijing_MB_NoPU_2D_Tight_Official.root", CorrectionPath.c_str()));
  } else if (CollisionSystem == "pO") {
    std::cout << "ERROR: pO tracking efficiency not implemented yet"
              << std::endl; // FIXME: implement pO tracking efficiency
  }

  // EVENT SELECTION EFFICIENCY
  EvtSelCorrection *EventSelectionEfficiency_Nominal = nullptr;
  EvtSelCorrection *EventSelectionEfficiency_Tight = nullptr;
  EvtSelCorrection *EventSelectionEfficiency_Loose = nullptr;
  std::string EvtSelCorrectionFile_Nominal = "";
  std::string EvtSelCorrectionFile_Tight = "";
  std::string EvtSelCorrectionFile_Loose = "";
  if (CollisionSystem == "pp") { // USING OO FILES FOR PPREF ANALYSIS AS DUMMIES
    // FILL WITH FILES.
  } else if (CollisionSystem == "OO") {
    EvtSelCorrectionFile_Nominal = Form("%s/OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root", CorrectionPath.c_str());
    EvtSelCorrectionFile_Loose = Form("%s/OORAA_MULT_EFFICIENCY_HIJING_HF10AND.root", CorrectionPath.c_str());
    EvtSelCorrectionFile_Tight = Form("%s/OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root", CorrectionPath.c_str());
  } else if (CollisionSystem == "NeNe") { // USING OO FILES FOR NE-NE ANALYSIS AS DUMMIES
    EvtSelCorrectionFile_Nominal = Form("%s/NENERAA_MULT_EFFICIENCY_HIJING_HF13AND.root", CorrectionPath.c_str());
    EvtSelCorrectionFile_Loose = Form("%s/NENERAA_MULT_EFFICIENCY_HIJING_HF10AND.root", CorrectionPath.c_str());
    EvtSelCorrectionFile_Tight = Form("%s/NENERAA_MULT_EFFICIENCY_HIJING_HF19AND.root", CorrectionPath.c_str());
  } else if (CollisionSystem == "pO") { // USING OO FILES FOR pO ANALYSIS AS DUMMIES
    // FILL WITH FILES.
  }
  EventSelectionEfficiency_Nominal = new EvtSelCorrection(true, EvtSelCorrectionFile_Nominal);
  EventSelectionEfficiency_Tight = new EvtSelCorrection(true, EvtSelCorrectionFile_Tight);
  EventSelectionEfficiency_Loose = new EvtSelCorrection(true, EvtSelCorrectionFile_Loose);

  // MC REWEIGHTING
  MCReweighting *MC_VZReweight = nullptr;
  MCReweighting *MC_MultReweight = nullptr;
  MCReweighting *MC_TrkPtReweight = nullptr;
  MCReweighting *MC_TrkDCAReweight = nullptr;
  std::string MC_CorrectionFile = "";
  if (IsData == false) {
    if (CollisionSystem == "pp") {
    } // DUMMY TO KEEP NULLPTR
    else if (CollisionSystem == "OO") {
      MC_CorrectionFile = Form("%s/OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root", CorrectionPath.c_str());
    } else if (CollisionSystem == "NeNe") {
      MC_CorrectionFile = Form("%s/NENERAA_MULT_EFFICIENCY_HIJING_HF13AND.root", CorrectionPath.c_str());
    } 
    else if (CollisionSystem == "pO") {
    } // DUMMY TO KEEP NULLPTR
    MC_VZReweight = new MCReweighting(true, MC_CorrectionFile.c_str(), "VZReweight");
    MC_MultReweight = new MCReweighting(true, MC_CorrectionFile.c_str(), "MultReweight");
    MC_TrkPtReweight = new MCReweighting(true, MC_CorrectionFile.c_str(), "TrkPtReweight");
    MC_TrkDCAReweight = new MCReweighting(true, MC_CorrectionFile.c_str(), "TrkDCAReweight");
  }

  // TRACK SPECIES REWEIGHTING
  MCReweighting *TrkSpeciesWeight_pp = nullptr;
  MCReweighting *TrkSpeciesWeight_dNdEta40 = nullptr;
  MCReweighting *TrkSpeciesWeight_dNdEta100 = nullptr;
  std::string Species_CorrectionFile = "";
  if (IsData == true) {
    if (CollisionSystem == "pp") {
      Species_CorrectionFile = Form("%s/ParticleSpeciesCorrectionFactorsOO.root", CorrectionPath.c_str());
    }
    else if (CollisionSystem == "OO") {
      Species_CorrectionFile = Form("%s/ParticleSpeciesCorrectionFactorsOO.root", CorrectionPath.c_str());
    }
    else if (CollisionSystem == "NeNe") {
      Species_CorrectionFile = Form("%s/ParticleSpeciesCorrectionFactorsOO.root", CorrectionPath.c_str());
    } else if (CollisionSystem == "pO") {
    } // DUMMY
    TrkSpeciesWeight_pp = new MCReweighting(true, Species_CorrectionFile.c_str(), "correctionFactor_PPref");
    TrkSpeciesWeight_dNdEta40 = new MCReweighting(true, Species_CorrectionFile.c_str(), "correctionFactor_dNdeta40");
    TrkSpeciesWeight_dNdEta100 = new MCReweighting(true, Species_CorrectionFile.c_str(), "correctionFactor_dNdeta100");
  }

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for OO RAA analysis :) (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  ChargedHadronRAATreeMessenger MChargedHadronRAA;
  MChargedHadronRAA.SetBranch(&Tree, saveTriggerBitsMode, DebugMode, includeFSCandPPSMode);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile); // hiEvtAnalyzer/HiTree
    PPTrackTreeMessenger MTrack(InputFile, "ppTracks/trackTree");
    GenParticleTreeMessenger MGen(InputFile);      // HiGenParticleAna/hi
    SkimTreeMessenger MSkim(InputFile);            // skimanalysis/HltTree
    HFAdcMessenger MHFAdc(InputFile);              // HFAdcana/adc
    PFTreeMessenger MPF(InputFile, PFTreeName);    // particleFlowAnalyser/pftree
    ZDCTreeMessenger MZDC(InputFile, ZDCTreeName); // zdcanalyzer/zdcrechit
    TriggerTreeMessenger MTrigger(InputFile);      // hltanalysis/HltTree
    PPSTreeMessenger MPPS(InputFile, PPSTreeName); // ppsanalyzer/ppstracks
    FSCTreeMessenger MFSC(InputFile, FSCTreeName); // fscanalyzer/fscdigi
    // METFilterTreeMessenger MMETFilter(InputFile); // l1MetFilterRecoTree/MetFilterRecoTree

    int EntryCount = MEvent.GetEntries() * Fraction;
    ProgressBar Bar(cout, EntryCount);
    if (!HideProgressBar) {
      Bar.SetStyle(-1);
    }
    /////////////////////////////////
    //////// Main Event Loop ////////
    /////////////////////////////////
    for (int iE = 0; iE < EntryCount; iE++) {
      if (!HideProgressBar && (EntryCount < 300 || (iE % (EntryCount / 250)) == 0)) {
        Bar.Update(iE);
        Bar.Print();
      }
      MEvent.GetEntry(iE);
      MGen.GetEntry(iE);
      MTrack.GetEntry(iE);
      MSkim.GetEntry(iE);

      MHFAdc.GetEntry(iE);
      MZDC.GetEntry(iE);
      MPPS.GetEntry(iE);
      MFSC.GetEntry(iE);
      MTrigger.GetEntry(iE);
      // MMETFilter.GetEntry(iE);

      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////
      MChargedHadronRAA.sampleType = sampleType;
      MChargedHadronRAA.Run = MEvent.Run;
      MChargedHadronRAA.Lumi = MEvent.Lumi;
      MChargedHadronRAA.Event = MEvent.Event;
      MChargedHadronRAA.hiHF_pf = MEvent.hiHF_pf;
      MChargedHadronRAA.hiHFPlus_pf = MEvent.hiHFPlus_pf;
      MChargedHadronRAA.hiHFMinus_pf = MEvent.hiHFMinus_pf;
      if (CollisionSystem == "OO" || CollisionSystem == "NeNe" ||
          CollisionSystem == "pO") /// should we expect a pO centrality?
        MChargedHadronRAA.hiBin = getHiBinFromhiHF(MEvent.hiHF_pf);
      else
        MChargedHadronRAA.hiBin = -9999;
      MChargedHadronRAA.Ncoll = MEvent.Ncoll;
      MChargedHadronRAA.Npart = MEvent.Npart;
      ////////////////////////////
      ////////// Vertex //////////
      ////////////////////////////

      int BestVertex = -1;
      for (int i = 0; i < MTrack.nVtx; i++) {
        if (BestVertex < 0 || MTrack.ptSumVtx->at(i) > MTrack.ptSumVtx->at(BestVertex))
          BestVertex = i;
      }
      if (BestVertex >= 0) {
        MChargedHadronRAA.VX = MTrack.xVtx->at(BestVertex);
        MChargedHadronRAA.VY = MTrack.yVtx->at(BestVertex);
        MChargedHadronRAA.VZ = MTrack.zVtx->at(BestVertex);
        MChargedHadronRAA.VXError = MTrack.xErrVtx->at(BestVertex);
        MChargedHadronRAA.VYError = MTrack.yErrVtx->at(BestVertex);
        MChargedHadronRAA.VZError = MTrack.zErrVtx->at(BestVertex);
        MChargedHadronRAA.isFakeVtx = MTrack.isFakeVtx->at(BestVertex);
        MChargedHadronRAA.ptSumVtx = MTrack.ptSumVtx->at(BestVertex);
        MChargedHadronRAA.nTracksVtx = MTrack.nTracksVtx->at(BestVertex);
        MChargedHadronRAA.chi2Vtx = MTrack.chi2Vtx->at(BestVertex);
        MChargedHadronRAA.ndofVtx = MTrack.ndofVtx->at(BestVertex);
        MChargedHadronRAA.bestVtxIndx = BestVertex;
      }
      MChargedHadronRAA.nVtx = MTrack.nVtx;
      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////
      MChargedHadronRAA.ZDCsumPlus = MZDC.sumPlus;
      MChargedHadronRAA.ZDCsumMinus = MZDC.sumMinus;
      MChargedHadronRAA.ClusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;
      MChargedHadronRAA.PVFilter = MSkim.PVFilter;
      MChargedHadronRAA.mMaxL1HFAdcPlus = MHFAdc.mMaxL1HFAdcPlus;
      MChargedHadronRAA.mMaxL1HFAdcMinus = MHFAdc.mMaxL1HFAdcMinus;
      MChargedHadronRAA.HFEMaxPlus = MEvent.hiHFPlus_pfle1;
      MChargedHadronRAA.HFEMaxPlus2 = MEvent.hiHFPlus_pfle2;
      MChargedHadronRAA.HFEMaxPlus3 = MEvent.hiHFPlus_pfle3;
      MChargedHadronRAA.HFEMaxMinus = MEvent.hiHFMinus_pfle1;
      MChargedHadronRAA.HFEMaxMinus2 = MEvent.hiHFMinus_pfle2;
      MChargedHadronRAA.HFEMaxMinus3 = MEvent.hiHFMinus_pfle3;
      MChargedHadronRAA.VZ_pf = MEvent.vz;

      if (CollisionSystem == "pp") {
        if (IsData == true) {
          int HLT_PPRefZeroBias_v6 = MTrigger.CheckTriggerStartWith("HLT_PPRefZeroBias_v6");
          MChargedHadronRAA.HLT_PPRefZeroBias_v6 = MTrigger.CheckTriggerStartWith("HLT_PPRefZeroBias_v6");
          if (ApplyTriggerRejection == 1 && HLT_PPRefZeroBias_v6 == 0)
            continue;
        } // end of CollisionSystem == "pp" and IsData == true
        else {
        } // end of CollisionSystem == "pp" && IsData == false
      } // end of CollisionSystem == "pp"
      else if (CollisionSystem == "OO" || CollisionSystem == "NeNe") {
        if (IsData == true) {
          int HLT_OxyZeroBias_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyZeroBias_v1");
          int HLT_MinimumBiasHF_OR_BptxAND_v1 = MTrigger.CheckTriggerStartWith("HLT_MinimumBiasHF_OR_BptxAND_v1");
          MChargedHadronRAA.HLT_OxySingleJet16_ZDC1nAsymXOR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleJet16_ZDC1nAsymXOR_v1");
          MChargedHadronRAA.HLT_OxySingleJet16_ZDC1nXOR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleJet16_ZDC1nXOR_v1");
          MChargedHadronRAA.HLT_OxySingleJet24_ZDC1nAsymXOR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleJet24_ZDC1nAsymXOR_v1");
          MChargedHadronRAA.HLT_OxySingleJet24_ZDC1nXOR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleJet24_ZDC1nXOR_v1");
          MChargedHadronRAA.HLT_OxyZDC1nOR_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyZDC1nOR_v1");
          MChargedHadronRAA.HLT_OxyZeroBias_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyZeroBias_v1");
          MChargedHadronRAA.HLT_MinimumBiasHF_OR_BptxAND_v1 =
              MTrigger.CheckTriggerStartWith("HLT_MinimumBiasHF_OR_BptxAND_v1");
          MChargedHadronRAA.HLT_OxyL1SingleJet20_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyL1SingleJet20_v1");
          if (ApplyTriggerRejection == 1 && HLT_OxyZeroBias_v1 == 0) {
            continue;
          }
          if (ApplyTriggerRejection == 2 && HLT_MinimumBiasHF_OR_BptxAND_v1 == 0) {
            continue;
          }
        } // end of CollisionSystem == "OO" && IsData == true
      } // end of CollisionSystem == "OO"
      else if (CollisionSystem == "pO") {
        if (IsData == true) {
          MChargedHadronRAA.HLT_OxyZeroBias_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyZeroBias_v1");
          MChargedHadronRAA.HLT_OxyZDC1nOR_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyZDC1nOR_v1");
          MChargedHadronRAA.HLT_OxySingleMuOpen_NotMBHF2OR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleMuOpen_NotMBHF2OR_v1");
          MChargedHadronRAA.HLT_OxySingleJet8_ZDC1nAsymXOR_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxySingleJet8_ZDC1nAsymXOR_v1");
          MChargedHadronRAA.HLT_OxyNotMBHF2_v1 = MTrigger.CheckTriggerStartWith("HLT_OxyNotMBHF2_v1");
          MChargedHadronRAA.HLT_OxyZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxyZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v1");
          MChargedHadronRAA.HLT_OxyZeroBias_MinPixelCluster400_v1 =
              MTrigger.CheckTriggerStartWith("HLT_OxyZeroBias_MinPixelCluster400_v1");
          MChargedHadronRAA.HLT_MinimumBiasHF_OR_BptxAND_v1 =
              MTrigger.CheckTriggerStartWith("HLT_MinimumBiasHF_OR_BptxAND_v1");
          MChargedHadronRAA.HLT_MinimumBiasHF_AND_BptxAND_v1 =
              MTrigger.CheckTriggerStartWith("HLT_MinimumBiasHF_AND_BptxAND_v1");
        } // end of CollisionSystem == "pO" && IsData == true
      } // end of CollisionSystem == "pO"

      //////////////////////////////////////////////////
      ///// Build L1 emulated trigger selections  //////
      //////////////////////////////////////////////////
      if (CollisionSystem != "pp" && includeL1EMU) {
        MChargedHadronRAA.passL1HFAND_16_Online = checkHFANDCondition(MChargedHadronRAA, 16., 16., true);
        MChargedHadronRAA.passL1HFOR_16_Online = checkHFORCondition(MChargedHadronRAA, 16., true);
        MChargedHadronRAA.passL1HFAND_14_Online = checkHFANDCondition(MChargedHadronRAA, 14., 14., true);
        MChargedHadronRAA.passL1HFOR_14_Online = checkHFORCondition(MChargedHadronRAA, 14., true);
      } else {
        MChargedHadronRAA.passL1HFAND_16_Online = false;
        MChargedHadronRAA.passL1HFOR_16_Online = false;
        MChargedHadronRAA.passL1HFAND_14_Online = false;
        MChargedHadronRAA.passL1HFOR_14_Online = false;
      }
      ////////////////////////////////////////////////////
      //////// Fill Baseline evt. sel filters ////////////
      ////////////////////////////////////////////////////
      bool passBaselineEventSelection = false;
      if (CollisionSystem == "pp") { // PVfilter, PV position within 15 cm
        passBaselineEventSelection = getBaselinePPEventSel(MChargedHadronRAA);
      } else if (CollisionSystem == "OO" || CollisionSystem == "NeNe" ||
                 CollisionSystem == "pO") { // PVfilter, PV position within 15 cm, ClusterCompatibilityFilter
        passBaselineEventSelection = getBaselineOOEventSel(MChargedHadronRAA);
      }
      MChargedHadronRAA.passBaselineEventSelection = passBaselineEventSelection;
      ///////////////////////////////////////////
      ////////// Offline HF conditions //////////
      ///////////////////////////////////////////

      bool passHFAND_10_Offline = false;
      bool passHFAND_13_Offline = false;
      bool passHFAND_19_Offline = false;
      bool passOR_OfflineHFAND = false;

      if (CollisionSystem != "pp") {
        passHFAND_10_Offline = checkHFANDCondition(MChargedHadronRAA, 10., 10., false);
        passHFAND_13_Offline = checkHFANDCondition(MChargedHadronRAA, 13., 13., false);
        passHFAND_19_Offline = checkHFANDCondition(MChargedHadronRAA, 19., 19., false);

        if (ApplyEventRejection && IsData == true) {
          passOR_OfflineHFAND = passHFAND_10_Offline || passHFAND_13_Offline || passHFAND_19_Offline;
          if (passOR_OfflineHFAND == false) {
            continue; // reject event if none of the HFAND conditions are met
          }
        }
      }
      MChargedHadronRAA.passHFAND_10_Offline = passHFAND_10_Offline;
      MChargedHadronRAA.passHFAND_13_Offline = passHFAND_13_Offline;
      MChargedHadronRAA.passHFAND_19_Offline = passHFAND_19_Offline;

      // loop over tracks
      int NTrack = MTrack.nTrk;
      MChargedHadronRAA.nTrk = NTrack;
      int locMultiplicityEta2p4 = 0;
      int locMultiplicityEta1p0 = 0;
      float leadingTrackPtEta1p0 = 0.;

      for (int iTrack = 0; iTrack < NTrack; iTrack++) {
        if (MTrack.trkPt->at(iTrack) < 0.4 || abs(MTrack.trkEta->at(iTrack)) > 2.4)
          continue; // skip tracks with pT < 0.4 GeV/c and |eta| > 2.4

        bool isSelectedTrackNominal = false;
        bool isSelectedTrackLoose = false;
        bool isSelectedTrackTight = false;

        // KD: apply track selection criteria that matches that used for efficiency files, if available

        isSelectedTrackNominal = MTrack.PassChargedHadronPPOONeNe2025StandardCuts(iTrack);
        isSelectedTrackLoose = MTrack.PassChargedHadronPPOONeNe2025LooseCuts(iTrack);
        isSelectedTrackTight = MTrack.PassChargedHadronPPOONeNe2025TightCuts(iTrack);

        bool isSelectedTrackORCondition = (isSelectedTrackNominal || isSelectedTrackLoose || isSelectedTrackTight);

        if (ApplyTrackRejection == true && isSelectedTrackORCondition == false)
          continue;

        if (isSelectedTrackNominal) {
          locMultiplicityEta2p4++;
          if (abs(MTrack.trkEta->at(iTrack)) < 1.0) {
            locMultiplicityEta1p0++;
            if (MTrack.trkPt->at(iTrack) > leadingTrackPtEta1p0) {
              leadingTrackPtEta1p0 = MTrack.trkPt->at(iTrack);
            }
          }
        }

        float trkPt = MTrack.trkPt->at(iTrack);
        if (trkPt < rejectTracksBelowPt && rejectTracksBelowPtEnabled)
          continue;
        float trkEta = MTrack.trkEta->at(iTrack);
        float trkPhi = MTrack.trkPhi->at(iTrack);
        float trkPtError = MTrack.trkPtError->at(iTrack);
        bool highPurity = MTrack.highPurity->at(iTrack);
        float trkDxyAssociatedVtx = MTrack.trkDxyAssociatedVtx->at(iTrack);
        float trkDzAssociatedVtx = MTrack.trkDzAssociatedVtx->at(iTrack);
        float trkDxyErrAssociatedVtx = MTrack.trkDxyErrAssociatedVtx->at(iTrack);
        float trkDzErrAssociatedVtx = MTrack.trkDzErrAssociatedVtx->at(iTrack);
        int trkAssociatedVtxIndx = MTrack.trkAssociatedVtxIndx->at(iTrack);
        char trkCharge = MTrack.trkCharge->at(iTrack);
        char trkNHits = MTrack.trkNHits->at(iTrack);
        char trkNPixHits = MTrack.trkNPixHits->at(iTrack);
        char trkNLayers = MTrack.trkNLayers->at(iTrack);
        float trkNormChi2 = MTrack.trkNormChi2->at(iTrack);
        float pfEnergy = MTrack.pfEnergy->at(iTrack);

        MChargedHadronRAA.trkPassChargedHadron_Nominal->push_back(isSelectedTrackNominal);
        MChargedHadronRAA.trkPassChargedHadron_Loose->push_back(isSelectedTrackLoose);
        MChargedHadronRAA.trkPassChargedHadron_Tight->push_back(isSelectedTrackTight);
        MChargedHadronRAA.trkEta->push_back(trkEta);
        MChargedHadronRAA.trkPt->push_back(trkPt);
        MChargedHadronRAA.trkPhi->push_back(trkPhi);
        MChargedHadronRAA.trkPtError->push_back(trkPtError);
        MChargedHadronRAA.highPurity->push_back(highPurity);
        MChargedHadronRAA.trkDxyAssociatedVtx->push_back(trkDxyAssociatedVtx);
        MChargedHadronRAA.trkDzAssociatedVtx->push_back(trkDzAssociatedVtx);
        MChargedHadronRAA.trkDxyErrAssociatedVtx->push_back(trkDxyErrAssociatedVtx);
        MChargedHadronRAA.trkDzErrAssociatedVtx->push_back(trkDzErrAssociatedVtx);
        MChargedHadronRAA.trkAssociatedVtxIndx->push_back(trkAssociatedVtxIndx);
        MChargedHadronRAA.trkCharge->push_back(trkCharge);
        MChargedHadronRAA.trkNHits->push_back(trkNHits);
        MChargedHadronRAA.trkNPixHits->push_back(trkNPixHits);
        MChargedHadronRAA.trkNLayers->push_back(trkNLayers);
        MChargedHadronRAA.trkNormChi2->push_back(trkNormChi2);
        MChargedHadronRAA.pfEnergy->push_back(pfEnergy);

        double TrackCorrection = 1;
        // efficiency correction component of total track weight
        if (CollisionSystem == "pp") {
          MChargedHadronRAA.trackingEfficiency_Nominal->push_back(TrackEfficiencyPP2024->getCorrection(trkPt, trkEta));
          // 2024 ppref, DCA loose and tight
          MChargedHadronRAA.trackingEfficiency_Loose->push_back(
              TrackEfficiencyPP2024_DCALoose->getCorrection(trkPt, trkEta));
          MChargedHadronRAA.trackingEfficiency_Tight->push_back(
              TrackEfficiencyPP2024_DCATight->getCorrection(trkPt, trkEta));
        } else if (CollisionSystem == "OO") {
          MChargedHadronRAA.trackingEfficiency_Nominal->push_back(TrackEfficiencyOO2025->getCorrection(trkPt, trkEta));
          MChargedHadronRAA.trackingEfficiency_Loose->push_back(
              TrackEfficiencyOO2025_DCALoose->getCorrection(trkPt, trkEta));
          MChargedHadronRAA.trackingEfficiency_Tight->push_back(
              TrackEfficiencyOO2025_DCATight->getCorrection(trkPt, trkEta));
        } else if (CollisionSystem == "NeNe") {
          MChargedHadronRAA.trackingEfficiency_Nominal->push_back(
              TrackEfficiencyNeNe2025->getCorrection(trkPt, trkEta));
          MChargedHadronRAA.trackingEfficiency_Loose->push_back(
              TrackEfficiencyNeNe2025_DCALoose->getCorrection(trkPt, trkEta));
          MChargedHadronRAA.trackingEfficiency_Tight->push_back(
              TrackEfficiencyNeNe2025_DCATight->getCorrection(trkPt, trkEta));
        } else if (CollisionSystem == "pO") {
          MChargedHadronRAA.trackingEfficiency_Nominal->push_back(0.); // No correction for pO
          MChargedHadronRAA.trackingEfficiency_Loose->push_back(0.);   // No correction for pO
          MChargedHadronRAA.trackingEfficiency_Tight->push_back(0.);   // No correction for pO
        }
        // total track correction calculation
        if (CollisionSystem == "pp")
          TrackCorrection = TrackEfficiencyPP2024->getCorrection(trkPt, trkEta);
        else if (CollisionSystem == "OO")
          TrackCorrection = TrackEfficiencyOO2025->getCorrection(trkPt, trkEta);
        else if (CollisionSystem == "NeNe")
          TrackCorrection = TrackEfficiencyNeNe2025->getCorrection(trkPt, trkEta);
        else if (CollisionSystem == "pO")
          TrackCorrection = 0.; // No correction for pO
        MChargedHadronRAA.trackWeight->push_back(TrackCorrection);

        /// SPECIES DEPENDENT CORRECTION
        if (IsData == true) {
          if (CollisionSystem == "pp") {
            MChargedHadronRAA.TrkSpeciesWeight_pp->push_back(
                (trkPt > 20 || !TrkSpeciesWeight_pp) ? 1 : TrkSpeciesWeight_pp->getCorrection(trkPt));
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta40->push_back(0);
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta100->push_back(0);
          }
          if (CollisionSystem == "OO" || CollisionSystem == "NeNe") {
            MChargedHadronRAA.TrkSpeciesWeight_pp->push_back(
                (trkPt > 20 || !TrkSpeciesWeight_pp) ? 1 : TrkSpeciesWeight_pp->getCorrection(trkPt));
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta40->push_back(
                (trkPt > 20 || !TrkSpeciesWeight_dNdEta40) ? 1 : TrkSpeciesWeight_dNdEta40->getCorrection(trkPt));
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta100->push_back(
                (trkPt > 20 || !TrkSpeciesWeight_dNdEta100) ? 1 : TrkSpeciesWeight_dNdEta100->getCorrection(trkPt));
          }
          if (CollisionSystem == "pO") {
            // SPECIES - returns factor of 1 if track pT > 20 GeV. No correction applied here
            MChargedHadronRAA.TrkSpeciesWeight_pp->push_back(0);
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta40->push_back(0);
            MChargedHadronRAA.TrkSpeciesWeight_dNdEta100->push_back(0);
          }
        }

        // MC TRACK LEVEL REWEIGHT CORRECTION
        float MC_TrkPtWeight = 0;
        if (IsData == false && MC_TrkPtReweight != nullptr) {
          MC_TrkPtWeight = MC_TrkPtReweight->getCorrection(trkPt);
        }
        MChargedHadronRAA.MC_TrkPtReweight->push_back(MC_TrkPtWeight);

        float MC_TrkDCAWeight = 0;
        if (IsData == false && MC_TrkDCAReweight != nullptr) {
          MC_TrkDCAWeight = MC_TrkDCAReweight->getCorrection(trkDxyAssociatedVtx);
        }
        MChargedHadronRAA.MC_TrkDCAReweight->push_back(MC_TrkDCAWeight);

      } // end of loop over tracks (gen or reco)

      MChargedHadronRAA.leadingPtEta1p0_sel = leadingTrackPtEta1p0;
      MChargedHadronRAA.multiplicityEta1p0 = locMultiplicityEta1p0;
      MChargedHadronRAA.multiplicityEta2p4 = locMultiplicityEta2p4;

      // EVENT SELECTION CORRECTION
      float eventEfficiencyCorrection_Nominal = -1.0;
      if (EventSelectionEfficiency_Nominal != nullptr) {
        eventEfficiencyCorrection_Nominal =
            EventSelectionEfficiency_Nominal->getCorrection(MChargedHadronRAA.multiplicityEta2p4);
      }
      float eventEfficiencyCorrection_Tight = -1.0;
      if (EventSelectionEfficiency_Tight != nullptr) {
        eventEfficiencyCorrection_Tight =
            EventSelectionEfficiency_Tight->getCorrection(MChargedHadronRAA.multiplicityEta2p4);
      }
      float eventEfficiencyCorrection_Loose = -1.0;
      if (EventSelectionEfficiency_Loose != nullptr) {
        eventEfficiencyCorrection_Loose =
            EventSelectionEfficiency_Loose->getCorrection(MChargedHadronRAA.multiplicityEta2p4);
      }

      MChargedHadronRAA.eventEfficiencyWeight_Nominal =
          (EventSelectionEfficiency_Nominal != nullptr) ? eventEfficiencyCorrection_Nominal : -1.0;
      MChargedHadronRAA.eventEfficiencyWeight_Tight =
          (EventSelectionEfficiency_Tight != nullptr) ? eventEfficiencyCorrection_Tight : -1.0;
      MChargedHadronRAA.eventEfficiencyWeight_Loose =
          (EventSelectionEfficiency_Loose != nullptr) ? eventEfficiencyCorrection_Loose : -1.0;

      float VZWeightMC = 0;
      float MultWeightMC = 0;
      if (IsData == false && MC_VZReweight != nullptr) {
        VZWeightMC = MC_VZReweight->getCorrection(MChargedHadronRAA.VZ);
      }
      if (IsData == false && MC_MultReweight != nullptr) {
        MultWeightMC = MC_MultReweight->getCorrection(MChargedHadronRAA.multiplicityEta2p4);
      }

      MChargedHadronRAA.MC_VZReweight = VZWeightMC;
      MChargedHadronRAA.MC_MultReweight = MultWeightMC;

      ////////////////////////////
      ///// Debug variables //////
      ////////////////////////////

      if (DebugMode) {
        for (int iDebVtx = 0; iDebVtx < MTrack.nVtx; iDebVtx++) {
          MChargedHadronRAA.AllxVtx->push_back(MTrack.xVtx->at(iDebVtx));
          MChargedHadronRAA.AllyVtx->push_back(MTrack.yVtx->at(iDebVtx));
          MChargedHadronRAA.AllzVtx->push_back(MTrack.zVtx->at(iDebVtx));
          MChargedHadronRAA.AllxVtxError->push_back(MTrack.xErrVtx->at(iDebVtx));
          MChargedHadronRAA.AllyVtxError->push_back(MTrack.yErrVtx->at(iDebVtx));
          MChargedHadronRAA.AllzVtxError->push_back(MTrack.zErrVtx->at(iDebVtx));
          MChargedHadronRAA.AllisFakeVtx->push_back(MTrack.isFakeVtx->at(iDebVtx));
          MChargedHadronRAA.AllnTracksVtx->push_back(MTrack.nTracksVtx->at(iDebVtx));
          MChargedHadronRAA.Allchi2Vtx->push_back(MTrack.chi2Vtx->at(iDebVtx));
          MChargedHadronRAA.AllndofVtx->push_back(MTrack.ndofVtx->at(iDebVtx));
          MChargedHadronRAA.AllptSumVtx->push_back(MTrack.ptSumVtx->at(iDebVtx));
        }
      }

      ////////////////////////////
      /// PPS & FSC variables ////
      ////////////////////////////

      if (includeFSCandPPSMode) {
        // PPS variables
        if (MPPS.n > PPSMAXN) {
          std::cout << "ERROR: in the PPS tree of the forest n > PPSMAXN; skipping PPS information filling"
                    << std::endl;
        } else {
          for (int iPPS = 0; iPPS < MPPS.n; iPPS++) {
            fillPPSInfo(MChargedHadronRAA, MPPS, iPPS);
          }
        }

        // FSC variables
        if (MFSC.n > FSCMAXN) {
          std::cout << "ERROR: in the FSC tree of the forest n > FSCMAXN; skipping FSC information filling"
                    << std::endl;
        } else {
          for (int iFSC = 0; iFSC < MFSC.n; iFSC++) {
            fillFSCInfo(MChargedHadronRAA, MFSC, iFSC);
          }
        }
      }

      MChargedHadronRAA.FillEntry();
    }
    if (!HideProgressBar) {
      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();
    } // if (!HideProgressBar)
    InputFile.Close();
  }
  OutputFile.cd();
  Tree.Write();
  InfoTree.Write();

  OutputFile.Close();

  return 0;
}
