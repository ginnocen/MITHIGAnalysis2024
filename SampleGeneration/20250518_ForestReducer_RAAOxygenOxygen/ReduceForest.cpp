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
std::vector<float> GetMaxEnergyHF(PFTreeMessenger *M, double etaMin, double etaMax);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);
  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  bool DoGenLevel = CL.GetBool("DoGenLevel", false);
  bool IsData = CL.GetBool("IsData", false);
  bool IsPP = CL.GetBool("IsPP", false);
  int Year = CL.GetInt("Year", 2025);

  double Fraction = CL.GetDouble("Fraction", 1.00);
  int ApplyTriggerRejection = CL.GetInteger("ApplyTriggerRejection", 0);
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  bool ApplyTrackRejection = CL.GetBool("ApplyTrackRejection", true);
  // bool ApplyZDCGapRejection = CL.GetBool("ApplyZDCGapRejection", false);
  string TrackEfficiencyPath = (DoGenLevel == false) ? CL.Get("TrackEfficiencyPath") : "";
  // 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
  int sampleType = CL.GetInteger("sampleType", 0);
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcrechit");
  bool HideProgressBar = CL.GetBool("HideProgressBar", false);
  bool DebugMode = CL.GetBool("DebugMode", false);

  TrkEff2017pp *TrackEfficiencyPP2017 = nullptr;
  if (DoGenLevel == false) {
    if (IsPP == true && (Year == 2017)) // using 2017 pp data corrections
      TrackEfficiencyPP2017 = new TrkEff2017pp(false, TrackEfficiencyPath);
    else {
      cerr << endl;
      cerr << "Error in track efficiency!" << endl;
      cerr << "Data/Year combination (IsPP = " << IsPP << ", Year = " << Year << ") does not exist!" << endl;
      cerr << endl;
    }
  }

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for UPC Dzero analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  ChargedHadronRAATreeMessenger MChargedHadronRAA;
  MChargedHadronRAA.SetBranch(&Tree, DebugMode);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile); // hiEvtAnalyzer/HiTree
    PPTrackTreeMessenger MTrack(InputFile, "ppTracks/trackTree");
    GenParticleTreeMessenger MGen(InputFile);      // HiGenParticleAna/hi
    PFTreeMessenger MPF(InputFile, PFTreeName);    // particleFlowAnalyser/pftree
    SkimTreeMessenger MSkim(InputFile);            // skimanalysis/HltTree
    HFAdcMessenger MHFAdc(InputFile);              // HFAdcana/adc
    ZDCTreeMessenger MZDC(InputFile, ZDCTreeName); // zdcanalyzer/zdcrechit
    TriggerTreeMessenger MTrigger(InputFile);      // hltanalysis/HltTree
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
      MZDC.GetEntry(iE);
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
      if (IsPP == false)
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

      if (IsPP == true) {
        if (IsData == true) {
          int HLT_PPRefZeroBias_v6 = MTrigger.CheckTriggerStartWith("HLT_PPRefZeroBias_v6");
          if (ApplyTriggerRejection == 1 && HLT_PPRefZeroBias_v6 == 0)
            continue;
          if (ApplyEventRejection &&
              (MChargedHadronRAA.ClusterCompatibilityFilter == false || MChargedHadronRAA.PVFilter == false)) {
            continue;
          } // end of if on ApplyEventRejection
        } // end of IsData && IsPP
        else {
        } // end of IsPP && !IsData
      } // end of IsPP
      else { // !IsPP
        if (IsData == true) {
        } // end of !IsPP && IsData
        else { // !IsPP && !IsData
        }
      } // end of IsPP == false
      // Loop through the specified ranges for gapgammaN and gapNgamma
      // gammaN[4] and Ngamma[4] are nominal selection criteria
      std::vector<float> EMaxHFPlus_top3 = GetMaxEnergyHF(&MPF, 3.0, 5.2);
      std::vector<float> EMaxHFMinus_top3 = GetMaxEnergyHF(&MPF, -5.2, -3.0);
      MChargedHadronRAA.HFEMaxPlus = EMaxHFPlus_top3[0];
      MChargedHadronRAA.HFEMaxPlus2 = EMaxHFPlus_top3[1];
      MChargedHadronRAA.HFEMaxPlus3 = EMaxHFPlus_top3[2];
      MChargedHadronRAA.HFEMaxMinus = EMaxHFMinus_top3[0];
      MChargedHadronRAA.HFEMaxMinus2 = EMaxHFMinus_top3[1];
      MChargedHadronRAA.HFEMaxMinus3 = EMaxHFMinus_top3[2];

      // loop over tracks
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
          if (ApplyTrackRejection == true && MTrack.PassChargedHadronPPStandardCuts(iTrack) == false)
            continue;
          if (abs(MTrack.trkEta->at(iTrack)) < 1.0 && MTrack.trkPt->at(iTrack) > leadingTrackPtEta1p0) {
            leadingTrackPtEta1p0 = MTrack.trkPt->at(iTrack);
          }
        } // end of if on DoGenLevel == false
        float trkEta = DoGenLevel ? MGen.Eta->at(iTrack) : MTrack.trkEta->at(iTrack);
        float trkPt = DoGenLevel ? MGen.PT->at(iTrack) : MTrack.trkPt->at(iTrack);
        float trkPhi = DoGenLevel ? MGen.Phi->at(iTrack) : MTrack.trkPhi->at(iTrack);
        float trkPtError = DoGenLevel ? 0 : MTrack.trkPtError->at(iTrack);
        bool highPurity = DoGenLevel ? true : MTrack.highPurity->at(iTrack);
        float trkDxyAssociatedVtx = DoGenLevel ? -9999 : MTrack.trkDxyAssociatedVtx->at(iTrack);
        float trkDzAssociatedVtx = DoGenLevel ? -9999 : MTrack.trkDzAssociatedVtx->at(iTrack);
        float trkDxyErrAssociatedVtx = DoGenLevel ? -9999 : MTrack.trkDxyErrAssociatedVtx->at(iTrack);
        float trkDzErrAssociatedVtx = DoGenLevel ? -9999 : MTrack.trkDzErrAssociatedVtx->at(iTrack);
        int trkAssociatedVtxIndx = DoGenLevel ? -1 : MTrack.trkAssociatedVtxIndx->at(iTrack);
        char trkCharge = DoGenLevel ? char(MGen.Charge->at(iTrack)) : MTrack.trkCharge->at(iTrack);
        char trkNHits = DoGenLevel ? static_cast<char>(-1) : MTrack.trkNHits->at(iTrack);
        char trkNPixHits = DoGenLevel ? static_cast<char>(-1) : MTrack.trkNPixHits->at(iTrack);
        char trkNLayers = DoGenLevel ? static_cast<char>(-1) : MTrack.trkNLayers->at(iTrack);
        float trkNormChi2 = DoGenLevel ? -1 : MTrack.trkNormChi2->at(iTrack);
        float pfEnergy = DoGenLevel ? -9999 : MTrack.pfEnergy->at(iTrack);
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
        if (DoGenLevel == false) {
          if (IsPP == true && (Year == 2017))
            TrackCorrection = TrackEfficiencyPP2017->getCorrection(trkPt, trkEta);
        } // end of if on DoGenLevel == false
        MChargedHadronRAA.trackWeight->push_back(TrackCorrection);
      } // end of loop over tracks (gen or reco)
      MChargedHadronRAA.leadingPtEta1p0_sel = leadingTrackPtEta1p0;

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
std::vector<float> GetMaxEnergyHF(PFTreeMessenger *M, double etaMin = 3., double etaMax = 5.) {
  if (M == nullptr)
    return {-9999., -9999., -9999.};
  if (M->Tree == nullptr)
    return {-9999., -9999., -9999.};

  std::vector<float> EMax_vec = {-1, -1, -1};

  for (int iPF = 0; iPF < M->ID->size(); iPF++) {
    if ((M->ID->at(iPF) == 6 || M->ID->at(iPF) == 7) && M->Eta->at(iPF) > etaMin && M->Eta->at(iPF) < etaMax) {
      float currentE = M->E->at(iPF);

      if (currentE > EMax_vec[0]) {
        EMax_vec[2] = EMax_vec[1];
        EMax_vec[1] = EMax_vec[0];
        EMax_vec[0] = currentE;
      } else if (currentE > EMax_vec[1]) {
        EMax_vec[2] = EMax_vec[1];
        EMax_vec[1] = currentE;
      } else if (currentE > EMax_vec[2]) {
        EMax_vec[2] = currentE;
      }
    }
  }

  return EMax_vec;
}
