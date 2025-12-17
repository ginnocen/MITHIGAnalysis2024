// ===================================================
// Script Based on Yi Chen Reducer script
// Yi Chen (Vanderbilt) Yi.Chen@cern.ch
// https://github.com/FHead/PhysicsZHadronEEC
// ===================================================

#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "TrackResidualCorrector.h"
#include "tnp_weight.h"
#include "trackingEfficiency2023PbPb.h"

#include "include/LambdaCpkpiSelection.h"
using namespace LambdaCpkpiSelection;
#include "include/PIDScoring.h"

bool logical_or_vectBool(std::vector<bool>* vec) {
    return std::any_of(vec->begin(), vec->end(), [](bool b) { return b; });
}

// Helper function to convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

int main(int argc, char *argv[]);
double GetMaxEnergyHF(PFTreeMessenger *M, double etaMin, double etaMax);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  // bool DoGenLevel                    = CL.GetBool("DoGenLevel", true);
  bool IsData = CL.GetBool("IsData", false);
  bool IsGammaNMCtype = CL.GetBool("IsGammaNMCtype", true); // This is only meaningful when IsData==false. gammaN: BeamA, Ngamma: BeamB
  int Year = CL.GetInt("Year", 2023);
  bool DoPID = CL.GetBool("DoPID", true);
  auto RootPID = CL.Get("RootPID", "../../CommonCode/root/DzeroUPC_dedxMap.root");
  
  double Fraction = CL.GetDouble("Fraction", 1.00);
  float ZDCMinus1nThreshold = CL.GetDouble("ZDCMinus1nThreshold", 1000.);
  float ZDCPlus1nThreshold = CL.GetDouble("ZDCPlus1nThreshold", 1100.);
  int ApplyTriggerRejection = CL.GetInteger("ApplyTriggerRejection", 0);
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  bool ApplyZDCGapRejection = CL.GetBool("ApplyZDCGapRejection", false);

  // options for how to reject non-selected D candidates (case-insensitive)
  // "NO":            keep all D's
  // "OR":            keep D's that pass the OR logic of all D selections
  // "Nominal":       reject !DpassCutNominal
  // "Loose":         reject !DpassCutLoose
  // "SystDsvpvSig":  reject !DpassCutSystDsvpvSig
  // "SystDtrkPt":    reject !DpassCutSystDtrkPt
  // "SystDalpha":    reject !DpassCutSystDalpha
  // "SystDdtheta":   reject !DpassCutSystDdtheta
  // "SystDchi2cl":   reject !DpassCutSystDchi2cl
  string ApplyDRejection = toLower(CL.Get("ApplyDRejection", "no"));

  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcrechit");
  bool HideProgressBar = CL.GetBool("HideProgressBar", true);
  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for UPC Lambda_c->p+K+pi analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  LambdaCpkpiUPCTreeMessenger MLambdaCUPC;
  MLambdaCUPC.SetBranch(&Tree);

  // Allowed ApplyDRejection in lowercase
  std::set<std::string> allowedApplyDRejection = {"or", "nominal", "loose", "passystdsvpvsig", "passystdtrkpt", "passystdalpha", "passystddtheta", "passystdchi2cl", "no"};
  // Validate the argument
  if (allowedApplyDRejection.find(ApplyDRejection) != allowedApplyDRejection.end()) {
    std::cout << "Candidate filtering criterion: " << ApplyDRejection << std::endl;
    // Add your program logic here
  } else {
    std::cerr << "[Warning] Invalid ApplyDRejection. Set to NO rejection. Allowed ApplyDRejection are: ";
    for (const auto& ele : allowedApplyDRejection) {
        std::cout << ele << ", ";
    }
    std::cout << std::endl;
    ApplyDRejection = "no";
  }

  TF1 *fdedxPionCenter = 0, *fdedxPionSigmaLo = 0, *fdedxPionSigmaHi = 0,
    *fdedxKaonCenter = 0, *fdedxKaonSigmaLo = 0, *fdedxKaonSigmaHi = 0,
    *fdedxProtCenter = 0, *fdedxProtSigmaLo = 0, *fdedxProtSigmaHi = 0;
  if (DoPID) {
    std::cout<<"PID functions from: "<<RootPID<<std::endl;
    auto dedxFunctions = ImportPIDRoot(RootPID.c_str());
    fdedxPionCenter  = dedxFunctions[0];
    fdedxPionSigmaLo = dedxFunctions[1];
    fdedxPionSigmaHi = dedxFunctions[2];
    fdedxKaonCenter  = dedxFunctions[3];
    fdedxKaonSigmaLo = dedxFunctions[4];
    fdedxKaonSigmaHi = dedxFunctions[5];
    fdedxProtCenter  = dedxFunctions[6];
    fdedxProtSigmaLo = dedxFunctions[7];
    fdedxProtSigmaHi = dedxFunctions[8];
  }
  
  for (const auto& InputFileName : InputFileNames) {
    auto* InputFile = TFile::Open(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile); // hiEvtAnalyzer/HiTree
    PbPbUPCTrackTreeMessenger MTrackPbPbUPC(InputFile, InputFile->Get("PbPbTracks/trackTree") ?
                                            "PbPbTracks/trackTree" :
                                            "ppTracks/trackTree");
    GenParticleTreeMessenger MGen(InputFile); // HiGenParticleAna/hi
    PFTreeMessenger MPF(InputFile, PFTreeName); // particleFlowAnalyser/pftree
    SkimTreeMessenger MSkim(InputFile); // skimanalysis/HltTree
    TriggerTreeMessenger MTrigger(InputFile); // hltanalysis/HltTree
    LambdaCpkpiTreeMessenger MLambdaC(InputFile); // Dfinder/ntLambdaCtopkpi
    DfinderGenTreeMessenger MDfinderGen(InputFile); // Dfinder/ntGen
    ZDCTreeMessenger MZDC(InputFile, ZDCTreeName); // zdcanalyzer/zdcrechit
    METFilterTreeMessenger MMETFilter(InputFile); // l1MetFilterRecoTree/MetFilterRecoTree
    
    int EntryCount = MEvent.GetEntries() * Fraction;
    ProgressBar Bar(cout, EntryCount);
    if (!HideProgressBar) {
      Bar.SetStyle(-1);
    }

    /////////////////////////////////
    //////// Main Event Loop ////////
    /////////////////////////////////

    for (int iE = 0; iE < EntryCount; iE++) {
      if (!HideProgressBar &&
        (EntryCount < 300 || (iE % (EntryCount / 250)) == 0)
      ) {
        Bar.Update(iE);
        Bar.Print();
      }

      MEvent.GetEntry(iE);
      MGen.GetEntry(iE);
      MTrackPbPbUPC.GetEntry(iE);
      MPF.GetEntry(iE);
      MSkim.GetEntry(iE);
      MTrigger.GetEntry(iE);
      MLambdaC.GetEntry(iE);
      if (IsData == false)
        MDfinderGen.GetEntry(iE);
      MZDC.GetEntry(iE);
      MLambdaCUPC.Clear();
      MMETFilter.GetEntry(iE);

      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MLambdaCUPC.Run = MEvent.Run;
      MLambdaCUPC.Lumi = MEvent.Lumi;
      MLambdaCUPC.Event = MEvent.Event;

      ////////////////////////////
      ////////// Vertex //////////
      ////////////////////////////

      int BestVertex = -1;

      for (int i = 0; i < MTrackPbPbUPC.nVtx; i++) {
        if (BestVertex < 0 || MTrackPbPbUPC.ptSumVtx->at(i) > MTrackPbPbUPC.ptSumVtx->at(BestVertex))
          BestVertex = i;
      }
      if (BestVertex >= 0) {
        MLambdaCUPC.VX = MTrackPbPbUPC.xVtx->at(BestVertex);
        MLambdaCUPC.VY = MTrackPbPbUPC.yVtx->at(BestVertex);
        MLambdaCUPC.VZ = MTrackPbPbUPC.zVtx->at(BestVertex);
        MLambdaCUPC.VXError = MTrackPbPbUPC.xErrVtx->at(BestVertex);
        MLambdaCUPC.VYError = MTrackPbPbUPC.yErrVtx->at(BestVertex);
        MLambdaCUPC.VZError = MTrackPbPbUPC.zErrVtx->at(BestVertex);
      }
      MLambdaCUPC.nVtx = MTrackPbPbUPC.nVtx;

      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////
      if (IsData == false) {
        MLambdaCUPC.Gsize = MDfinderGen.Gsize;
        for (int iDGen = 0; iDGen < MDfinderGen.Gsize; iDGen++) {
          MLambdaCUPC.Gpt->push_back(MDfinderGen.Gpt[iDGen]);
          MLambdaCUPC.Gy->push_back(MDfinderGen.Gy[iDGen]);
          bool isSignalGen =
              (MDfinderGen.GisSignal[iDGen] == 1 || MDfinderGen.GisSignal[iDGen] == 2) && MDfinderGen.Gpt[iDGen] > 0.;
          bool isPromptGen = MDfinderGen.GBAncestorpdgId[iDGen] == 0;
          bool isFeeddownGen = (MDfinderGen.GBAncestorpdgId[iDGen] >= 500 && MDfinderGen.GBAncestorpdgId[iDGen] < 600) ||
                               (MDfinderGen.GBAncestorpdgId[iDGen] > -600 && MDfinderGen.GBAncestorpdgId[iDGen] <= -500);
          MLambdaCUPC.GisSignalCalc->push_back(isSignalGen);
          MLambdaCUPC.GisSignalCalcPrompt->push_back(isSignalGen && isPromptGen);
          MLambdaCUPC.GisSignalCalcFeeddown->push_back(isSignalGen && isFeeddownGen);
        }
      }
      if (IsData == true) {
        if (Year == 2023) {
          int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023 =
              MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000");
          int HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_2023 =
              MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000");
          int HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023 =
              MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400");
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023 =
              MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000");
          bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023 == 1 ||
                           HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023 == 1;
          bool isL1ZDCXORJet8 = HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023 == 1 ||
                                HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_2023 == 1;
          MLambdaCUPC.isL1ZDCOr = isL1ZDCOr;
          MLambdaCUPC.isL1ZDCXORJet8 = isL1ZDCXORJet8;
          MLambdaCUPC.isL1ZDCXORJet12 = false;
          MLambdaCUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData && (isL1ZDCOr == false && isL1ZDCXORJet8 == false)) continue;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2024){
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13");
          int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2");
          int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000");
          bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 == 1 || HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 == 1;
          MLambdaCUPC.isL1ZDCOr = isL1ZDCOr;
          MLambdaCUPC.isL1ZDCXORJet8 = false;
          MLambdaCUPC.isL1ZDCXORJet12 = false;
          MLambdaCUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData) std::cout << "Trigger rejection ZDCOR || ZDCXORJet8 not implemented for 2024" << std::endl;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2025) {
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000");
          int HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400");
          int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000");
          int HLT_HIUPC_SingleJet12_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet12_ZDC1nXOR_MaxPixelCluster10000");
          int HLT_HIUPC_SingleJet12_ZDC1nAsymXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet12_ZDC1nAsymXOR_MaxPixelCluster10000");
          int HLT_HIUPC_SingleJet16_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet16_ZDC1nXOR_MaxPixelCluster10000");
          int HLT_HIUPC_SingleJet16_ZDC1nAsymXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet16_ZDC1nAsymXOR_MaxPixelCluster10000");
          bool isL1ZDCOr =  HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 == 1;
          bool isL1ZDCXORJet12 = HLT_HIUPC_SingleJet12_ZDC1nXOR_MaxPixelCluster10000 == 1 ||
                                HLT_HIUPC_SingleJet12_ZDC1nAsymXOR_MaxPixelCluster10000 == 1;
          bool isL1ZDCXORJet16 = HLT_HIUPC_SingleJet16_ZDC1nXOR_MaxPixelCluster10000 == 1 ||
                                HLT_HIUPC_SingleJet16_ZDC1nAsymXOR_MaxPixelCluster10000 == 1;
          // [Note] If we prescale the HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 in the upcoming run, we will change to the following
          //   bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400 == 1 ||
          //                    HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 == 1;
          MLambdaCUPC.isL1ZDCOr = isL1ZDCOr;
          MLambdaCUPC.isL1ZDCOr_Min400 = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000;
          MLambdaCUPC.isL1ZDCOr_Max400 = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400;
          MLambdaCUPC.isL1ZDCXORJet8 = false;
          MLambdaCUPC.isL1ZDCXORJet12 = HLT_HIUPC_SingleJet12_ZDC1nXOR_MaxPixelCluster10000;
          MLambdaCUPC.isL1ZDCXORJet16 = HLT_HIUPC_SingleJet16_ZDC1nXOR_MaxPixelCluster10000;

          // [Note] If we prescale the HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v5 in the upcoming run, we will change to the following
          // bool isZeroBias = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v16") == 1 ||
          //                 MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v15") == 1 ||
          //                 MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v5") == 1;
          MLambdaCUPC.isZeroBias = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MaxPixelCluster10000");
          MLambdaCUPC.isZeroBias_Min400 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000");
          MLambdaCUPC.isZeroBias_Max400 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400");
          if (ApplyTriggerRejection == 1 && IsData && (MLambdaCUPC.isL1ZDCOr || MLambdaCUPC.isL1ZDCOr_Min400 || MLambdaCUPC.isL1ZDCOr_Max400 || MLambdaCUPC.isL1ZDCXORJet12)==false) continue;
          if (ApplyTriggerRejection == 2 && IsData && (MLambdaCUPC.isL1ZDCOr || MLambdaCUPC.isL1ZDCOr_Min400 || MLambdaCUPC.isL1ZDCOr_Max400)==false) continue;
        }
      }
      if (IsData == true) {
        MLambdaCUPC.ZDCsumPlus = MZDC.sumPlus;
        MLambdaCUPC.ZDCsumMinus = MZDC.sumMinus;
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1 && MMETFilter.cscTightHalo2015Filter;
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        if (ApplyEventRejection && IsData && (selectedBkgFilter == false || selectedVtxFilter == false)) continue;
        MLambdaCUPC.selectedBkgFilter = selectedBkgFilter;
        MLambdaCUPC.selectedVtxFilter = selectedVtxFilter;
        bool ZDCgammaN = (MZDC.sumMinus > ZDCMinus1nThreshold && MZDC.sumPlus < ZDCPlus1nThreshold);
        bool ZDCNgamma = (MZDC.sumMinus < ZDCMinus1nThreshold && MZDC.sumPlus > ZDCPlus1nThreshold);
        MLambdaCUPC.ZDCgammaN = ZDCgammaN;
        MLambdaCUPC.ZDCNgamma = ZDCNgamma;
      } // end of if (IsData == true)
      else { // if (IsData == false)
        // MLambdaCUPC.ZDCsumPlus = MZDC.sumPlus;
        // MLambdaCUPC.ZDCsumMinus = MZDC.sumMinus;
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1; // METFilter always true for MC
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        MLambdaCUPC.selectedBkgFilter = selectedBkgFilter;
        MLambdaCUPC.selectedVtxFilter = selectedVtxFilter;
        bool ZDCgammaN =  IsGammaNMCtype;
        bool ZDCNgamma = !IsGammaNMCtype;
        MLambdaCUPC.ZDCgammaN = ZDCgammaN;
        MLambdaCUPC.ZDCNgamma = ZDCNgamma;
      } // end of if (IsData == false)

      // Loop through the specified ranges for gapgammaN and gapNgamma
      // gammaN[4] and Ngamma[4] are nominal selection criteria
      float EMaxHFPlus = GetMaxEnergyHF(&MPF, 3., 5.2);
      float EMaxHFMinus = GetMaxEnergyHF(&MPF, -5.2, -3.);
      MLambdaCUPC.HFEMaxPlus = EMaxHFPlus;
      MLambdaCUPC.HFEMaxMinus = EMaxHFMinus;
      bool gapgammaN = EMaxHFPlus < 9.2;
      bool gapNgamma = EMaxHFMinus < 8.6;
      MLambdaCUPC.gapgammaN = gapgammaN;
      MLambdaCUPC.gapNgamma = gapNgamma;
      bool gammaN_default = MLambdaCUPC.ZDCgammaN && gapgammaN;
      bool Ngamma_default = MLambdaCUPC.ZDCNgamma && gapNgamma;
      // if (ApplyZDCGapRejection && IsData && gammaN_default == false && Ngamma_default == false) continue;
      for (const auto& gapgammaN_threshold : MLambdaCUPC.gapEThresh_gammaN) {
        bool gapgammaN_ = GetMaxEnergyHF(&MPF, 3.0, 5.2) < gapgammaN_threshold;
        bool gammaN_ = MLambdaCUPC.ZDCgammaN && gapgammaN_;
        MLambdaCUPC.gammaN->push_back(gammaN_);
      }
      for (const auto& gapNgamma_threshold : MLambdaCUPC.gapEThresh_Ngamma) {
        bool gapNgamma_ = GetMaxEnergyHF(&MPF, -5.2, -3.0) < gapNgamma_threshold;
        bool Ngamma_ = MLambdaCUPC.ZDCNgamma && gapNgamma_;
        MLambdaCUPC.Ngamma->push_back(Ngamma_);
      }
      /////// cut on the loosest rapidity gap selection
      if (ApplyZDCGapRejection && IsData && MLambdaCUPC.gammaN_EThreshLoose() == false && MLambdaCUPC.Ngamma_EThreshLoose() == false) continue;

      int nTrackInAcceptanceHP = 0;
      for (int iTrack = 0; iTrack < MTrackPbPbUPC.nTrk; iTrack++) {
        if (MTrackPbPbUPC.trkPt->at(iTrack) <= 0.5)
          continue;
        if (fabs(MTrackPbPbUPC.trkEta->at(iTrack)) >= 2.4)
          continue;
        if (MTrackPbPbUPC.highPurity->at(iTrack) == false)
          continue;
        nTrackInAcceptanceHP++;
      }
      MLambdaCUPC.nTrackInAcceptanceHP = nTrackInAcceptanceHP;

      /////////////////////////////////////
      /////////////    D loop    //////////
      /////////////////////////////////////
      int countSelD = 0;
      for (int iD = 0; iD < MLambdaC.Dsize; iD++) {
        bool DpassCutNominal_           = DpassCutNominal(MLambdaC, iD);
        bool DpassCutLoose_             = DpassCutLoose(MLambdaC, iD);
        bool DpassCutSystDsvpvSig_      = DpassCutSystDsvpvSig(MLambdaC, iD);
        bool DpassCutSystDtrkPt_        = DpassCutSystDtrkPt(MLambdaC, iD);
        bool DpassCutSystDalpha_        = DpassCutSystDalpha(MLambdaC, iD);
        bool DpassCutSystDdtheta_       = DpassCutSystDdtheta(MLambdaC, iD);
        bool DpassCutSystDchi2cl_       = DpassCutSystDchi2cl(MLambdaC, iD);
        if (IsData)
        {
          if (ApplyDRejection=="or")
          {
            if (!DpassCutNominal_       &&
                !DpassCutLoose_         &&
                !DpassCutSystDsvpvSig_  &&
                !DpassCutSystDtrkPt_    &&
                !DpassCutSystDalpha_    &&
                !DpassCutSystDdtheta_   &&
                !DpassCutSystDchi2cl_ ) continue;
          }
          else if (ApplyDRejection=="nominal"         && !DpassCutNominal_) continue;
          else if (ApplyDRejection=="loose"           && !DpassCutLoose_) continue;
          else if (ApplyDRejection=="passystdsvpvsig" && !DpassCutSystDsvpvSig_) continue;
          else if (ApplyDRejection=="passystdtrkpt"   && !DpassCutSystDtrkPt_) continue;
          else if (ApplyDRejection=="passystdalpha"   && !DpassCutSystDalpha_) continue;
          else if (ApplyDRejection=="passystddtheta"  && !DpassCutSystDdtheta_) continue;
          else if (ApplyDRejection=="passystdchi2cl"  && !DpassCutSystDchi2cl_) continue;
        }
        countSelD++;
        MLambdaCUPC.Dpt->push_back(             MLambdaC.Dpt[iD]);
        MLambdaCUPC.Dy->push_back(              MLambdaC.Dy[iD]);
        MLambdaCUPC.Dmass->push_back(           MLambdaC.Dmass[iD]);
        MLambdaCUPC.Dchi2cl->push_back(         MLambdaC.Dchi2cl[iD]);
        MLambdaCUPC.DsvpvDistance->push_back(   MLambdaC.DsvpvDistance[iD]);
        MLambdaCUPC.DsvpvDisErr->push_back(     MLambdaC.DsvpvDisErr[iD]);
        MLambdaCUPC.DsvpvDistance_2D->push_back(MLambdaC.DsvpvDistance_2D[iD]);
        MLambdaCUPC.DsvpvDisErr_2D->push_back(  MLambdaC.DsvpvDisErr_2D[iD]);
        MLambdaCUPC.Dalpha->push_back(          MLambdaC.Dalpha[iD]);
        MLambdaCUPC.Ddtheta->push_back(         MLambdaC.Ddtheta[iD]);
        
        MLambdaCUPC.Dtrk1P->push_back(          MLambdaC.Dtrk1P[iD]);
        MLambdaCUPC.Dtrk1Pt->push_back(         MLambdaC.Dtrk1Pt[iD]);
        MLambdaCUPC.Dtrk1PtErr->push_back(      MLambdaC.Dtrk1PtErr[iD]);
        MLambdaCUPC.Dtrk1Eta->push_back(        MLambdaC.Dtrk1Eta[iD]);
        MLambdaCUPC.Dtrk1dedx->push_back(       MLambdaC.Dtrk1dedx[iD]);
        MLambdaCUPC.Dtrk1MassHypo->push_back(   MLambdaC.Dtrk1MassHypo[iD]);
        MLambdaCUPC.Dtrk1PixelHit->push_back(   MLambdaC.Dtrk1PixelHit[iD]);
        MLambdaCUPC.Dtrk1StripHit->push_back(   MLambdaC.Dtrk1StripHit[iD]);
        if (MLambdaC.Dtrk1P[iD] < 2.5) // Only give valid PID for p_track < 2 GeV
        {
          MLambdaCUPC.Dtrk1PionScore->push_back(GetPIDScore(
            MLambdaC.Dtrk1P[iD], MLambdaC.Dtrk1dedx[iD],
            fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
          MLambdaCUPC.Dtrk1KaonScore->push_back(GetPIDScore(
            MLambdaC.Dtrk1P[iD], MLambdaC.Dtrk1dedx[iD],
            fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
          MLambdaCUPC.Dtrk1ProtScore->push_back(GetPIDScore(
            MLambdaC.Dtrk1P[iD], MLambdaC.Dtrk1dedx[iD],
            fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
        } else {
          MLambdaCUPC.Dtrk1PionScore->push_back(-999.);
          MLambdaCUPC.Dtrk1KaonScore->push_back(-999.);
          MLambdaCUPC.Dtrk1ProtScore->push_back(-999.);
        }
        
        MLambdaCUPC.Dtrk2P->push_back(          MLambdaC.Dtrk2P[iD]);
        MLambdaCUPC.Dtrk2Pt->push_back(         MLambdaC.Dtrk2Pt[iD]);
        MLambdaCUPC.Dtrk2PtErr->push_back(      MLambdaC.Dtrk2PtErr[iD]);
        MLambdaCUPC.Dtrk2Eta->push_back(        MLambdaC.Dtrk2Eta[iD]);
        MLambdaCUPC.Dtrk2dedx->push_back(       MLambdaC.Dtrk2dedx[iD]);
        MLambdaCUPC.Dtrk2MassHypo->push_back(   MLambdaC.Dtrk2MassHypo[iD]);
        MLambdaCUPC.Dtrk2PixelHit->push_back(   MLambdaC.Dtrk2PixelHit[iD]);
        MLambdaCUPC.Dtrk2StripHit->push_back(   MLambdaC.Dtrk2StripHit[iD]);
        if (MLambdaC.Dtrk2P[iD] < 2.5) // Only give valid PID for p_track < 2 GeV
        {
          MLambdaCUPC.Dtrk2PionScore->push_back(GetPIDScore(
            MLambdaC.Dtrk2P[iD], MLambdaC.Dtrk2dedx[iD],
            fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
          MLambdaCUPC.Dtrk2KaonScore->push_back(GetPIDScore(
            MLambdaC.Dtrk2P[iD], MLambdaC.Dtrk2dedx[iD],
            fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
          MLambdaCUPC.Dtrk2ProtScore->push_back(GetPIDScore(
            MLambdaC.Dtrk2P[iD], MLambdaC.Dtrk2dedx[iD],
            fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
        } else {
          MLambdaCUPC.Dtrk2PionScore->push_back(-999.);
          MLambdaCUPC.Dtrk2KaonScore->push_back(-999.);
          MLambdaCUPC.Dtrk2ProtScore->push_back(-999.);
        }
        
        MLambdaCUPC.Dtrk3P->push_back(          MLambdaC.Dtrk3P[iD]);
        MLambdaCUPC.Dtrk3Pt->push_back(         MLambdaC.Dtrk3Pt[iD]);
        MLambdaCUPC.Dtrk3PtErr->push_back(      MLambdaC.Dtrk3PtErr[iD]);
        MLambdaCUPC.Dtrk3Eta->push_back(        MLambdaC.Dtrk3Eta[iD]);
        MLambdaCUPC.Dtrk3dedx->push_back(       MLambdaC.Dtrk3dedx[iD]);
        MLambdaCUPC.Dtrk3MassHypo->push_back(   MLambdaC.Dtrk3MassHypo[iD]);
        MLambdaCUPC.Dtrk3PixelHit->push_back(   MLambdaC.Dtrk3PixelHit[iD]);
        MLambdaCUPC.Dtrk3StripHit->push_back(   MLambdaC.Dtrk3StripHit[iD]);
        if (MLambdaC.Dtrk3P[iD] < 2.5) // Only give valid PID for p_track < 2 GeV
        {
          MLambdaCUPC.Dtrk3PionScore->push_back(GetPIDScore(
            MLambdaC.Dtrk3P[iD], MLambdaC.Dtrk3dedx[iD],
            fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
          MLambdaCUPC.Dtrk3KaonScore->push_back(GetPIDScore(
            MLambdaC.Dtrk3P[iD], MLambdaC.Dtrk3dedx[iD],
            fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
          MLambdaCUPC.Dtrk3ProtScore->push_back(GetPIDScore(
            MLambdaC.Dtrk3P[iD], MLambdaC.Dtrk3dedx[iD],
            fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
        } else {
          MLambdaCUPC.Dtrk3PionScore->push_back(-999.);
          MLambdaCUPC.Dtrk3KaonScore->push_back(-999.);
          MLambdaCUPC.Dtrk3ProtScore->push_back(-999.);
        }
        
        MLambdaCUPC.DpassCutNominal->push_back(     DpassCutNominal_);
        MLambdaCUPC.DpassCutLoose->push_back(       DpassCutNominal_);
        MLambdaCUPC.DpassCutSystDsvpvSig->push_back(DpassCutSystDsvpvSig_);
        MLambdaCUPC.DpassCutSystDtrkPt->push_back(  DpassCutSystDtrkPt_);
        MLambdaCUPC.DpassCutSystDalpha->push_back(  DpassCutSystDalpha_);
        MLambdaCUPC.DpassCutSystDdtheta->push_back( DpassCutSystDdtheta_);
        MLambdaCUPC.DpassCutSystDchi2cl->push_back( DpassCutSystDchi2cl_);
        if (IsData == false) {
          // FIXME: Need to upate this for correct PID!
          MLambdaCUPC.Dgen->push_back(MLambdaC.Dgen[iD]);
          bool isSignalGenMatched = MLambdaC.Dgen[iD] == 23333 && MLambdaC.Dgenpt[iD] > 0.;
          bool isPromptGenMatched = MLambdaC.DgenBAncestorpdgId[iD] == 0;
          bool isFeeddownGenMatched = (MLambdaC.DgenBAncestorpdgId[iD] >= 500 && MLambdaC.DgenBAncestorpdgId[iD] < 600) ||
                                      (MLambdaC.DgenBAncestorpdgId[iD] > -600 && MLambdaC.DgenBAncestorpdgId[iD] <= -500);
          MLambdaCUPC.DisSignalCalc->push_back(isSignalGenMatched);
          MLambdaCUPC.DisSignalCalcPrompt->push_back(isSignalGenMatched && isPromptGenMatched);
          MLambdaCUPC.DisSignalCalcFeeddown->push_back(isSignalGenMatched && isFeeddownGenMatched);
        }
      }
      MLambdaCUPC.Dsize = countSelD;
      MLambdaCUPC.FillEntry();
    }
    if (!HideProgressBar) {
      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();
    }

    InputFile->Close();
    std::cout<<"Processed "<<EntryCount<<" events."<<std::endl;
  }

  OutputFile.cd();
  Tree.Write();
  InfoTree.Write();

  OutputFile.Close();

  return 0;
}

// ============================================================================ //
// Function to Retrieve Maximum Energy in HF Region within Specified Eta Range
// ============================================================================ //
double GetMaxEnergyHF(PFTreeMessenger *M, double etaMin = 3., double etaMax = 5.) {
  if (M == nullptr)
    return -1;
  if (M->Tree == nullptr)
    return -1;

  double EMax = 0;
  for (int iPF = 0; iPF < M->ID->size(); iPF++) {
    if ((M->ID->at(iPF) == 6 || M->ID->at(iPF) == 7) && M->Eta->at(iPF) > etaMin && M->Eta->at(iPF) < etaMax) {
      if (M->E->at(iPF) > EMax)
        EMax = M->E->at(iPF);
    }
  }
  return EMax;
}

