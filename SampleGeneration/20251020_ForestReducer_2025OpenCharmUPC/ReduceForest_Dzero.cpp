// ===================================================
// Script Based on Yi Chen Reducer script
// Yi Chen (Vanderbilt) Yi.Chen@cern.ch
// https://github.com/FHead/PhysicsZHadronEEC
// ===================================================

#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
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
#include "trackingEfficiency2017pp.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"

#include "include/DzeroSelection.h"
using namespace DzeroSelection;
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
  // "NO":              keep all D's
  // "OR":              keep D's that pass the OR logic of all D selections
  // "Nominal":         reject !DpassCutNominal
  // "Loose":           reject !DpassCutLoose
  // "SystDsvpvSig":    reject !DpassCutSystDsvpvSig
  // "SystDtrkPt":      reject !DpassCutSystDtrkPt
  // "SystDalpha":      reject !DpassCutSystDalpha
  // "SystDdtheta":     reject !DpassCutSystDdtheta
  // "SystDchi2cl":     reject !DpassCutSystDchi2cl
  // "PASOR":           keep D's that pass the OR logic of all PAS selections
  //                    (this is a tighter selection than "OR")
  // "PAS":             reject !DpassCut23PAS
  // "PASSystDsvpvSig": reject !DpassCut23PASSystDsvpvSig
  // "PASSystDtrkPt":   reject !DpassCut23PASSystDtrkPt
  // "PASSystDalpha":   reject !DpassCut23PASSystDalpha
  // "PASSystDchi2cl":  reject !DpassCut23PASSystDchi2cl
  string ApplyDRejection = toLower(CL.Get("ApplyDRejection", "no"));
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcrechit");
  bool HideProgressBar = CL.GetBool("HideProgressBar", false);
  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for UPC Dzero analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  DzeroUPCTreeMessenger MDzeroUPC;
  MDzeroUPC.SetBranch(&Tree);

  // Allowed ApplyDRejection in lowercase
  std::set<std::string> allowedApplyDRejection = {
    "no", "or", "nominal", "loose",
    "systdsvpvsig", "systdtrkpt", "systdalpha", "systddtheta", "systdchi2cl",
    "pasor", "pas",
    "passystdsvpvsig", "passystdtrkpt", "passystdalpha", "passystdchi2cl"
  };
  // Validate the argument
  if (allowedApplyDRejection.find(ApplyDRejection) != allowedApplyDRejection.end()) {
    std::cout << "D filtering criterion: " << ApplyDRejection << std::endl;
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
    // Use PbPbTracks/trackTree as the default, else ppTracks/trackTree
    PbPbUPCTrackTreeMessenger MTrackPbPbUPC(InputFile, InputFile->Get("PbPbTracks/trackTree")?
                                                                     "PbPbTracks/trackTree":
                                                                     "ppTracks/trackTree"); // ppTracks/trackTree
    GenParticleTreeMessenger MGen(InputFile); // HiGenParticleAna/hi
    PFTreeMessenger MPF(InputFile, PFTreeName); // particleFlowAnalyser/pftree
    SkimTreeMessenger MSkim(InputFile); // skimanalysis/HltTree
    TriggerTreeMessenger MTrigger(InputFile); // hltanalysis/HltTree
    DzeroTreeMessenger MDzero(InputFile); // Dfinder/ntDkpi
    DfinderGenTreeMessenger MDzeroGen(InputFile); // Dfinder/ntGen
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
      MDzero.GetEntry(iE);
      if (IsData == false)
        MDzeroGen.GetEntry(iE);
      MZDC.GetEntry(iE);
      MDzeroUPC.Clear();
      MMETFilter.GetEntry(iE);

      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MDzeroUPC.Run = MEvent.Run;
      MDzeroUPC.Lumi = MEvent.Lumi;
      MDzeroUPC.Event = MEvent.Event;

      ////////////////////////////
      ////////// Vertex //////////
      ////////////////////////////

      int BestVertex = -1;

      for (int i = 0; i < MTrackPbPbUPC.nVtx; i++) {
        if (BestVertex < 0 || MTrackPbPbUPC.ptSumVtx->at(i) > MTrackPbPbUPC.ptSumVtx->at(BestVertex))
          BestVertex = i;
      }
      if (BestVertex >= 0) {
        MDzeroUPC.VX = MTrackPbPbUPC.xVtx->at(BestVertex);
        MDzeroUPC.VY = MTrackPbPbUPC.yVtx->at(BestVertex);
        MDzeroUPC.VZ = MTrackPbPbUPC.zVtx->at(BestVertex);
        MDzeroUPC.VXError = MTrackPbPbUPC.xErrVtx->at(BestVertex);
        MDzeroUPC.VYError = MTrackPbPbUPC.yErrVtx->at(BestVertex);
        MDzeroUPC.VZError = MTrackPbPbUPC.zErrVtx->at(BestVertex);
      }
      MDzeroUPC.nVtx = MTrackPbPbUPC.nVtx;

      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////

      if (IsData == false) {
        MDzeroUPC.Gsize = MDzeroGen.Gsize;
        for (int iDGen = 0; iDGen < MDzeroGen.Gsize; iDGen++) {
          MDzeroUPC.Gpt->push_back(MDzeroGen.Gpt[iDGen]);
          MDzeroUPC.Gy->push_back(MDzeroGen.Gy[iDGen]);
          bool isSignalGen =
              (MDzeroGen.GisSignal[iDGen] == 1 || MDzeroGen.GisSignal[iDGen] == 2) && MDzeroGen.Gpt[iDGen] > 0.;
          bool isPromptGen = MDzeroGen.GBAncestorpdgId[iDGen] == 0;
          bool isFeeddownGen = (MDzeroGen.GBAncestorpdgId[iDGen] >= 500 && MDzeroGen.GBAncestorpdgId[iDGen] < 600) ||
                               (MDzeroGen.GBAncestorpdgId[iDGen] > -600 && MDzeroGen.GBAncestorpdgId[iDGen] <= -500);
          MDzeroUPC.GisSignalCalc->push_back(isSignalGen);
          MDzeroUPC.GisSignalCalcPrompt->push_back(isSignalGen && isPromptGen);
          MDzeroUPC.GisSignalCalcFeeddown->push_back(isSignalGen && isFeeddownGen);
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
          MDzeroUPC.isL1ZDCOr = isL1ZDCOr;
          MDzeroUPC.isL1ZDCXORJet8 = isL1ZDCXORJet8;
          MDzeroUPC.isL1ZDCXORJet12 = false;
          MDzeroUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData && (isL1ZDCOr == false && isL1ZDCXORJet8 == false)) continue;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2024){
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13");
          int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2");
          int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000");
          bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 == 1 || HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 == 1;
          MDzeroUPC.isL1ZDCOr = isL1ZDCOr;
          MDzeroUPC.isL1ZDCXORJet8 = false;
          MDzeroUPC.isL1ZDCXORJet12 = false;
          MDzeroUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData) std::cout << "Trigger rejection ZDCOR || ZDCXORJet8 not implemented for 2024" << std::endl;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2025){
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
          MDzeroUPC.isL1ZDCOr = isL1ZDCOr;
          MDzeroUPC.isL1ZDCOr_Min400 = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000;
          MDzeroUPC.isL1ZDCOr_Max400 = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400;
          MDzeroUPC.isL1ZDCXORJet8 = false;
          MDzeroUPC.isL1ZDCXORJet12 = HLT_HIUPC_SingleJet12_ZDC1nXOR_MaxPixelCluster10000;
          MDzeroUPC.isL1ZDCXORJet16 = HLT_HIUPC_SingleJet16_ZDC1nXOR_MaxPixelCluster10000;

          // [Note] If we prescale the HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v5 in the upcoming run, we will change to the following
          // bool isZeroBias = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v16") == 1 ||
          //                 MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v15") == 1 ||
          //                 MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v5") == 1;
          MDzeroUPC.isZeroBias = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MaxPixelCluster10000");
          MDzeroUPC.isZeroBias_Min400 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000");
          MDzeroUPC.isZeroBias_Max400 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400");
          if (ApplyTriggerRejection == 1 && IsData && (MDzeroUPC.isL1ZDCOr || MDzeroUPC.isL1ZDCOr_Min400 || MDzeroUPC.isL1ZDCOr_Max400 || MDzeroUPC.isL1ZDCXORJet12)==false) continue;
          if (ApplyTriggerRejection == 2 && IsData && (MDzeroUPC.isL1ZDCOr || MDzeroUPC.isL1ZDCOr_Min400 || MDzeroUPC.isL1ZDCOr_Max400)==false) continue;
        }
      }
      if (IsData == true) {
        MDzeroUPC.ZDCsumPlus = MZDC.sumPlus;
        MDzeroUPC.ZDCsumMinus = MZDC.sumMinus;
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1 && MMETFilter.cscTightHalo2015Filter;
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        if (ApplyEventRejection && IsData && (selectedBkgFilter == false || selectedVtxFilter == false)) continue;
        MDzeroUPC.selectedBkgFilter = selectedBkgFilter;
        MDzeroUPC.selectedVtxFilter = selectedVtxFilter;
        bool ZDCgammaN = (MZDC.sumMinus > ZDCMinus1nThreshold && MZDC.sumPlus < ZDCPlus1nThreshold);
        bool ZDCNgamma = (MZDC.sumMinus < ZDCMinus1nThreshold && MZDC.sumPlus > ZDCPlus1nThreshold);
        MDzeroUPC.ZDCgammaN = ZDCgammaN;
        MDzeroUPC.ZDCNgamma = ZDCNgamma;
      } // end of if (IsData == true)
      else { // if (IsData == false)
        // MDzeroUPC.ZDCsumPlus = MZDC.sumPlus;
        // MDzeroUPC.ZDCsumMinus = MZDC.sumMinus;
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1; // METFilter always true for MC
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        MDzeroUPC.selectedBkgFilter = selectedBkgFilter;
        MDzeroUPC.selectedVtxFilter = selectedVtxFilter;
        bool ZDCgammaN =  IsGammaNMCtype;
        bool ZDCNgamma = !IsGammaNMCtype;
        MDzeroUPC.ZDCgammaN = ZDCgammaN;
        MDzeroUPC.ZDCNgamma = ZDCNgamma;
      } // end of if (IsData == false)

      // Loop through the specified ranges for gapgammaN and gapNgamma
      // gammaN[4] and Ngamma[4] are nominal selection criteria
      float EMaxHFPlus = GetMaxEnergyHF(&MPF, 3., 5.2);
      float EMaxHFMinus = GetMaxEnergyHF(&MPF, -5.2, -3.);
      MDzeroUPC.HFEMaxPlus = EMaxHFPlus;
      MDzeroUPC.HFEMaxMinus = EMaxHFMinus;
      bool gapgammaN = EMaxHFPlus < 9.2;
      bool gapNgamma = EMaxHFMinus < 8.6;
      MDzeroUPC.gapgammaN = gapgammaN;
      MDzeroUPC.gapNgamma = gapNgamma;
      bool gammaN_default = MDzeroUPC.ZDCgammaN && gapgammaN;
      bool Ngamma_default = MDzeroUPC.ZDCNgamma && gapNgamma;
      // if (ApplyZDCGapRejection && IsData && gammaN_default == false && Ngamma_default == false) continue;
      for (const auto& gapgammaN_threshold : MDzeroUPC.gapEThresh_gammaN) {
        bool gapgammaN_ = GetMaxEnergyHF(&MPF, 3.0, 5.2) < gapgammaN_threshold;
        bool gammaN_ = MDzeroUPC.ZDCgammaN && gapgammaN_;
        MDzeroUPC.gammaN->push_back(gammaN_);
      }
      for (const auto& gapNgamma_threshold : MDzeroUPC.gapEThresh_Ngamma) {
        bool gapNgamma_ = GetMaxEnergyHF(&MPF, -5.2, -3.0) < gapNgamma_threshold;
        bool Ngamma_ = MDzeroUPC.ZDCNgamma && gapNgamma_;
        MDzeroUPC.Ngamma->push_back(Ngamma_);
      }
      /////// cut on the loosest rapidity gap selection
      if (ApplyZDCGapRejection && IsData && MDzeroUPC.gammaN_EThreshLoose() == false && MDzeroUPC.Ngamma_EThreshLoose() == false) continue;

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
      MDzeroUPC.nTrackInAcceptanceHP = nTrackInAcceptanceHP;

      /////////////////////////////////////
      /////////////    D loop    //////////
      /////////////////////////////////////
      if (MDzero.Dsize == 20000) cout << "[WARNING] Event " << iE << " has max Dsize!" << endl;
      int countSelDzero = 0;
      for (int iD = 0; iD < MDzero.Dsize; iD++) {
        bool DpassCutNominal_           = DpassCutNominal(MDzero, iD);
        bool DpassCutSystDsvpvSig_      = DpassCutSystDsvpvSig(MDzero, iD);
        bool DpassCutSystDtrkPt_        = DpassCutSystDtrkPt(MDzero, iD);
        bool DpassCutSystDalpha_        = DpassCutSystDalpha(MDzero, iD);
        bool DpassCutSystDalphaDdtheta_ = DpassCutSystDalphaDdtheta(MDzero, iD);
        bool DpassCutSystDdtheta_       = DpassCutSystDdtheta(MDzero, iD);
        bool DpassCutSystDchi2cl_       = DpassCutSystDchi2cl(MDzero, iD);
        bool DpassCutLoose_             = DpassCutLoose(MDzero, iD);
        bool DpassCut23PAS_             = DpassCut23PAS(MDzero, iD);
        bool DpassCut23LowPt_           = DpassCut23LowPt(MDzero, iD);
        bool DpassCut23PASSystDsvpvSig_ = DpassCut23PASSystDsvpvSig(MDzero, iD);
        bool DpassCut23PASSystDtrkPt_   = DpassCut23PASSystDtrkPt(MDzero, iD);
        bool DpassCut23PASSystDalpha_   = DpassCut23PASSystDalpha(MDzero, iD);
        bool DpassCut23PASSystDchi2cl_  = DpassCut23PASSystDchi2cl(MDzero, iD);
        if (IsData)
        {
          if (ApplyDRejection=="or") { // OR for new cuts only
            if (!DpassCutNominal_           &&
                !DpassCutSystDsvpvSig_      &&
                !DpassCutSystDtrkPt_        &&
                !DpassCutSystDalpha_        &&
                !DpassCutSystDdtheta_       &&
                !DpassCutSystDalphaDdtheta_ &&
                !DpassCutSystDchi2cl_       &&
                !DpassCutLoose_ ) continue;
          }
          else if (ApplyDRejection=="nominal"         && !DpassCutNominal_) continue;
          else if (ApplyDRejection=="systdsvpvsig"    && !DpassCutSystDsvpvSig_) continue;
          else if (ApplyDRejection=="systdtrkpt"      && !DpassCutSystDtrkPt_) continue;
          else if (ApplyDRejection=="systdalpha"      && !DpassCutSystDalpha_) continue;
          else if (ApplyDRejection=="systddtheta"     && !DpassCutSystDdtheta_) continue;
          else if (ApplyDRejection=="systdchi2cl"     && !DpassCutSystDchi2cl_) continue;
          else if (ApplyDRejection=="loose"           && !DpassCutLoose_) continue;
          else if (ApplyDRejection=="pasor") { // OR for PAS23 cuts only
            if (!DpassCut23PAS_ &&
                !DpassCut23PASSystDsvpvSig_ &&
                !DpassCut23PASSystDtrkPt_ &&
                !DpassCut23PASSystDalpha_ &&
                !DpassCut23PASSystDchi2cl_ ) continue;
          }
          else if (ApplyDRejection=="pas"             && !DpassCut23PAS_) continue;
          else if (ApplyDRejection=="passystdsvpvsig" && !DpassCut23PASSystDsvpvSig_) continue;
          else if (ApplyDRejection=="passystdtrkpt"   && !DpassCut23PASSystDtrkPt_) continue;
          else if (ApplyDRejection=="passystdalpha"   && !DpassCut23PASSystDalpha_) continue;
          else if (ApplyDRejection=="passystdchi2cl"  && !DpassCut23PASSystDchi2cl_) continue;
        }
        countSelDzero++;
        MDzeroUPC.Dpt->push_back(             MDzero.Dpt[iD]);
        MDzeroUPC.Dy->push_back(              MDzero.Dy[iD]);
        MDzeroUPC.Dmass->push_back(           MDzero.Dmass[iD]);
        MDzeroUPC.Dchi2cl->push_back(         MDzero.Dchi2cl[iD]);
        MDzeroUPC.DsvpvDistance->push_back(   MDzero.DsvpvDistance[iD]);
        MDzeroUPC.DsvpvDisErr->push_back(     MDzero.DsvpvDisErr[iD]);
        MDzeroUPC.DsvpvDistance_2D->push_back(MDzero.DsvpvDistance_2D[iD]);
        MDzeroUPC.DsvpvDisErr_2D->push_back(  MDzero.DsvpvDisErr_2D[iD]);
//        MDzeroUPC.Dip3d->push_back(           MDzero.Dip3d[iD]);
//        MDzeroUPC.Dip3derr->push_back(        MDzero.Dip3derr[iD]);
        MDzeroUPC.Dalpha->push_back(          MDzero.Dalpha[iD]);
        MDzeroUPC.Ddtheta->push_back(         MDzero.Ddtheta[iD]);
        
        MDzeroUPC.Dtrk1P->push_back(          MDzero.Dtrk1P[iD]);
        MDzeroUPC.Dtrk1Pt->push_back(         MDzero.Dtrk1Pt[iD]);
        MDzeroUPC.Dtrk1PtErr->push_back(      MDzero.Dtrk1PtErr[iD]);
        MDzeroUPC.Dtrk1Eta->push_back(        MDzero.Dtrk1Eta[iD]);
        MDzeroUPC.Dtrk1dedx->push_back(       MDzero.Dtrk1dedx[iD]);
        MDzeroUPC.Dtrk1MassHypo->push_back(   MDzero.Dtrk1MassHypo[iD]);
        MDzeroUPC.Dtrk1PixelHit->push_back(   MDzero.Dtrk1PixelHit[iD]);
        MDzeroUPC.Dtrk1StripHit->push_back(   MDzero.Dtrk1StripHit[iD]);
        if (DoPID && MDzero.Dtrk1P[iD] < 2.5) // Only give valid PID for p_track < 2 GeV
        {
          MDzeroUPC.Dtrk1PionScore->push_back(GetPIDScore(
            MDzero.Dtrk1P[iD], MDzero.Dtrk1dedx[iD],
            fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
          MDzeroUPC.Dtrk1KaonScore->push_back(GetPIDScore(
            MDzero.Dtrk1P[iD], MDzero.Dtrk1dedx[iD],
            fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
          MDzeroUPC.Dtrk1ProtScore->push_back(GetPIDScore(
            MDzero.Dtrk1P[iD], MDzero.Dtrk1dedx[iD],
            fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
        } else {
          MDzeroUPC.Dtrk1PionScore->push_back(-999.);
          MDzeroUPC.Dtrk1KaonScore->push_back(-999.);
          MDzeroUPC.Dtrk1ProtScore->push_back(-999.);
        }
        
        MDzeroUPC.Dtrk2P->push_back(          MDzero.Dtrk2P[iD]);
        MDzeroUPC.Dtrk2Pt->push_back(         MDzero.Dtrk2Pt[iD]);
        MDzeroUPC.Dtrk2PtErr->push_back(      MDzero.Dtrk2PtErr[iD]);
        MDzeroUPC.Dtrk2Eta->push_back(        MDzero.Dtrk2Eta[iD]);
        MDzeroUPC.Dtrk2dedx->push_back(       MDzero.Dtrk2dedx[iD]);
        MDzeroUPC.Dtrk2MassHypo->push_back(   MDzero.Dtrk2MassHypo[iD]);
        MDzeroUPC.Dtrk2PixelHit->push_back(   MDzero.Dtrk2PixelHit[iD]);
        MDzeroUPC.Dtrk2StripHit->push_back(   MDzero.Dtrk2StripHit[iD]);
        if (DoPID && MDzero.Dtrk2P[iD] < 2.5) // Only do PID for p_track < 2 GeV
        {
          MDzeroUPC.Dtrk2PionScore->push_back(GetPIDScore(
            MDzero.Dtrk2P[iD], MDzero.Dtrk2dedx[iD],
            fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
          MDzeroUPC.Dtrk2KaonScore->push_back(GetPIDScore(
            MDzero.Dtrk2P[iD], MDzero.Dtrk2dedx[iD],
            fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
          MDzeroUPC.Dtrk2ProtScore->push_back(GetPIDScore(
            MDzero.Dtrk2P[iD], MDzero.Dtrk2dedx[iD],
            fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
        } else {
          MDzeroUPC.Dtrk2PionScore->push_back(-999.);
          MDzeroUPC.Dtrk2KaonScore->push_back(-999.);
          MDzeroUPC.Dtrk2ProtScore->push_back(-999.);
        }
        
        MDzeroUPC.DpassCutNominal->push_back(           DpassCutNominal_);
        MDzeroUPC.DpassCutSystDsvpvSig->push_back(      DpassCutSystDsvpvSig_);
        MDzeroUPC.DpassCutSystDtrkPt->push_back(        DpassCutSystDtrkPt_);
        MDzeroUPC.DpassCutSystDalpha->push_back(        DpassCutSystDalpha_);
        MDzeroUPC.DpassCutSystDdtheta->push_back(       DpassCutSystDdtheta_);
        MDzeroUPC.DpassCutSystDalphaDdtheta->push_back( DpassCutSystDalphaDdtheta_);
        MDzeroUPC.DpassCutSystDchi2cl->push_back(       DpassCutSystDchi2cl_);
        MDzeroUPC.DpassCutLoose->push_back(             DpassCutLoose_);
        MDzeroUPC.DpassCut23PAS->push_back(             DpassCut23PAS_);
        MDzeroUPC.DpassCut23PASSystDsvpvSig->push_back( DpassCut23PASSystDsvpvSig_);
        MDzeroUPC.DpassCut23PASSystDtrkPt->push_back(   DpassCut23PASSystDtrkPt_);
        MDzeroUPC.DpassCut23PASSystDalpha->push_back(   DpassCut23PASSystDalpha_);
        MDzeroUPC.DpassCut23PASSystDchi2cl->push_back(  DpassCut23PASSystDchi2cl_);
        if (IsData == false) {
          MDzeroUPC.Dgen->push_back(MDzero.Dgen[iD]);
          bool isSignalGenMatched = (MDzero.Dgen[iD] == 23333 || MDzero.Dgen[iD] == 41022 || MDzero.Dgen[iD] == 41044) && MDzero.Dgenpt[iD] > 0.;
          bool isPromptGenMatched = MDzero.DgenBAncestorpdgId[iD] == 0;
          bool isFeeddownGenMatched = (MDzero.DgenBAncestorpdgId[iD] >= 500 && MDzero.DgenBAncestorpdgId[iD] < 600) ||
                                      (MDzero.DgenBAncestorpdgId[iD] > -600 && MDzero.DgenBAncestorpdgId[iD] <= -500);
          MDzeroUPC.DisSignalCalc->push_back(isSignalGenMatched);
          MDzeroUPC.DisSignalCalcPrompt->push_back(isSignalGenMatched && isPromptGenMatched);
          MDzeroUPC.DisSignalCalcFeeddown->push_back(isSignalGenMatched && isFeeddownGenMatched);
        }
      }
      MDzeroUPC.Dsize = countSelDzero;
      MDzeroUPC.FillEntry();
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

