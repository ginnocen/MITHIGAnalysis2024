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

#include "TrackResidualCorrector.h"
#include "tnp_weight.h"
#include "trackingEfficiency2017pp.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"

#include "include/cent_OO_hijing_PF.h"

bool logical_or_vectBool(std::vector<bool> *vec) {
  return std::any_of(vec->begin(), vec->end(), [](bool b) { return b; });
}

// Helper function to convert a string to lowercase
std::string toLower(const std::string &str) {
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

  bool DoGenLevel = CL.GetBool("DoGenLevel", false);
  bool IsData = CL.GetBool("IsData", false);
  int Year = CL.GetInt("Year", 2025);

  double Fraction = CL.GetDouble("Fraction", 1.00);
  int ApplyTriggerRejection = CL.GetInteger("ApplyTriggerRejection", 0);
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  // bool ApplyZDCGapRejection = CL.GetBool("ApplyZDCGapRejection", false);

  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcrechit");
  bool HideProgressBar = CL.GetBool("HideProgressBar", false);

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for UPC Dzero analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  ChargedHadronRAATreeMessenger MChargedHadronRAA;
  MChargedHadronRAA.SetBranch(&Tree);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile);                            // hiEvtAnalyzer/HiTree
    PPTrackTreeMessenger MTrack(InputFile, "ppTracks/trackTree");
    GenParticleTreeMessenger MGen(InputFile);                          // HiGenParticleAna/hi
    PFTreeMessenger MPF(InputFile, PFTreeName);                        // particleFlowAnalyser/pftree
    SkimTreeMessenger MSkim(InputFile);                                // skimanalysis/HltTree
    HFAdcMessenger MHFAdc(InputFile);                                  // HFAdcana/adc
    // TriggerTreeMessenger MTrigger(InputFile); // hltanalysis/HltTree
    // ZDCTreeMessenger MZDC(InputFile, ZDCTreeName); // zdcanalyzer/zdcrechit
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
      MPF.GetEntry(iE);
      MSkim.GetEntry(iE);
      MHFAdc.GetEntry(iE);
      // MTrigger.GetEntry(iE);
      // MZDC.GetEntry(iE);
      // MMETFilter.GetEntry(iE);

      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MChargedHadronRAA.Run = MEvent.Run;
      MChargedHadronRAA.Lumi = MEvent.Lumi;
      MChargedHadronRAA.Event = MEvent.Event;
      MChargedHadronRAA.hiHF_pf = MEvent.hiHF_pf;
      MChargedHadronRAA.hiBin = getHiBinFromhiHF(MEvent.hiHF_pf);
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
      }
      MChargedHadronRAA.nVtx = MTrack.nVtx;
      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////

      if (IsData == true) {
        if (Year == 2025) {
          std::cout << "Year 2025" << std::endl;
          // int HLT_HIZB_ = MTrigger.CheckTriggerStartWith("HLT_HIZB");
          // MChargedHadronRAA.isHLT_HIZB = HLT_HIZB_;
          // if (ApplyTriggerRejection == 1 && IsData && (HLT_HIZB_ == false)) continue;
          // if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
          // MChargedHadronRAA.ZDCsumPlus = MZDC.sumPlus;
          // MChargedHadronRAA.ZDCsumMinus = MZDC.sumMinus;
          MChargedHadronRAA.ClusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;
          MChargedHadronRAA.PVFilter = MSkim.PVFilter;
          MChargedHadronRAA.mMaxL1HFAdcPlus = MHFAdc.mMaxL1HFAdcPlus;
          MChargedHadronRAA.mMaxL1HFAdcMinus = MHFAdc.mMaxL1HFAdcMinus;
        } // end of year == 2025
      } else { // if not data
               // MChargedHadronRAA.ZDCsumPlus = MZDC.sumPlus;
               // MChargedHadronRAA.ZDCsumMinus = MZDC.sumMinus;
        MChargedHadronRAA.ClusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;
        MChargedHadronRAA.PVFilter = MSkim.PVFilter;
        MChargedHadronRAA.mMaxL1HFAdcPlus = MHFAdc.mMaxL1HFAdcPlus;
        MChargedHadronRAA.mMaxL1HFAdcMinus = MHFAdc.mMaxL1HFAdcMinus;
      } // end of if not data

      // Loop through the specified ranges for gapgammaN and gapNgamma
      // gammaN[4] and Ngamma[4] are nominal selection criteria
      float EMaxHFPlus = GetMaxEnergyHF(&MPF, 3.0, 6.0);
      float EMaxHFMinus = GetMaxEnergyHF(&MPF, -6.0, -3.0);
      MChargedHadronRAA.HFEMaxPlus = EMaxHFPlus;
      MChargedHadronRAA.HFEMaxMinus = EMaxHFMinus;
      /*
      bool gapgammaN = EMaxHFPlus < 9.2;
      bool gapNgamma = EMaxHFMinus < 8.6;
      MChargedHadronRAA.gapgammaN = gapgammaN;
      MChargedHadronRAA.gapNgamma = gapNgamma;
      bool gammaN_default = MChargedHadronRAA.ZDCgammaN && gapgammaN;
      bool Ngamma_default = MChargedHadronRAA.ZDCNgamma && gapNgamma;
      // if (ApplyZDCGapRejection && IsData && gammaN_default == false && Ngamma_default == false) continue;
      for (const auto& gapgammaN_threshold : MChargedHadronRAA.gapEThresh_gammaN) {
        bool gapgammaN_ = GetMaxEnergyHF(&MPF, 3.0, 5.2) < gapgammaN_threshold;
        bool gammaN_ = MChargedHadronRAA.ZDCgammaN && gapgammaN_;
        MChargedHadronRAA.gammaN->push_back(gammaN_);
      }
      */
      /*
      for (const auto& gapNgamma_threshold : MChargedHadronRAA.gapEThresh_Ngamma) {
        bool gapNgamma_ = GetMaxEnergyHF(&MPF, -5.2, -3.0) < gapNgamma_threshold;
        bool Ngamma_ = MChargedHadronRAA.ZDCNgamma && gapNgamma_;
        MChargedHadronRAA.Ngamma->push_back(Ngamma_);
      }
      /////// cut on the loosest rapidity gap selection
      if (ApplyZDCGapRejection && IsData && MChargedHadronRAA.gammaN_EThreshLoose() == false &&
      MChargedHadronRAA.Ngamma_EThreshLoose() == false) continue;
      */
      int NTrack = DoGenLevel ? MGen.Mult : MTrack.nTrk;
      float leadingTrackPtEta1p0 = 0.;
      for (int iTrack = 0; iTrack < NTrack; iTrack++) {
        if (DoGenLevel == true) {
          if (MGen.DaughterCount->at(iTrack) > 0)
            continue;
          if (MGen.Charge->at(iTrack) == 0)
            continue;
        } // end of if on DoGenLevel == true
        if (DoGenLevel == false) {
          if (MTrack.highPurity->at(iTrack) == false)
            continue;
          if (abs(MTrack.trkEta->at(iTrack)) < 1.0 && MTrack.trkPt->at(iTrack) > leadingTrackPtEta1p0){
	    leadingTrackPtEta1p0 = MTrack.trkPt->at(iTrack);
          }
        } // end of if on DoGenLevel == false
        float trkEta = DoGenLevel ? MGen.Eta->at(iTrack) : MTrack.trkEta->at(iTrack);
        float trkPt = DoGenLevel ? MGen.PT->at(iTrack) : MTrack.trkPt->at(iTrack);
        bool highPurity = DoGenLevel ? true : MTrack.highPurity->at(iTrack);
        MChargedHadronRAA.trkEta->push_back(trkEta);
        MChargedHadronRAA.trkPt->push_back(trkPt);
        MChargedHadronRAA.highPurity->push_back(highPurity);
      } // end of loop over tracks (gen or reco)
      MChargedHadronRAA.leadingPtEta1p0_sel = leadingTrackPtEta1p0;
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
