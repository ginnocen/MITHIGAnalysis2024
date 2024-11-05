// ===================================================
// Script Based on Yi Chen Reducer script
// Yi Chen (Vanderbilt) Yi.Chen@cern.ch
// https://github.com/FHead/PhysicsZHadronEEC
// ===================================================

#include <iostream>
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

#include "include/DmesonSelection.h"

int main(int argc, char *argv[]);
double GetMaxEnergyHF(PFTreeMessenger *M, double etaMin, double etaMax);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  // bool DoGenLevel                    = CL.GetBool("DoGenLevel", true);
  bool   IsData        = CL.GetBool  ("IsData",     false); 
  int    Year          = CL.GetInt   ("Year",       2023);
  double MinDzeroPT    = CL.GetDouble("MinDzeroPT", 0.); 
  double MinTrackPT    = CL.GetDouble("MinTrackPT", 1);
  double Fraction      = CL.GetDouble("Fraction",   1.00);
  string PFTreeName    = CL.Get      ("PFTree",     "particleFlowAnalyser/pftree");
  string DGenTreeName  = CL.Get      ("DGenTree",   "Dfinder/ntGen");

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for UPC Dzero analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");

  DzeroUPCTreeMessenger MDzeroUPC;
  MDzeroUPC.SetBranch(&Tree);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile);
    PbPbUPCTrackTreeMessenger MTrackPbPbUPC(InputFile);
    GenParticleTreeMessenger MGen(InputFile);
    PFTreeMessenger MPF(InputFile, PFTreeName);
    SkimTreeMessenger MSkim(InputFile);
    TriggerTreeMessenger MTrigger(InputFile);
    DzeroTreeMessenger MDzero(InputFile);
    DzeroGenTreeMessenger MDzeroGen(InputFile, DGenTreeName);
    ZDCTreeMessenger MZDC(InputFile);
    METFilterTreeMessenger MFilter(InputFile);

    int EntryCount = MEvent.GetEntries() * Fraction;
    ProgressBar Bar(cout, EntryCount);
    Bar.SetStyle(-1);

    /////////////////////////////////
    //////// Main Event Loop ////////
    /////////////////////////////////

    for (int iE = 0; iE < EntryCount; iE++) {
      if (EntryCount < 300 || (iE % (EntryCount / 250)) == 0) {
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
      if (IsData == false) MDzeroGen.GetEntry(iE);
      MZDC.GetEntry(iE);
      MDzeroUPC.Clear();
      MFilter.GetEntry(iE);

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
        if (BestVertex < 0 || MTrackPbPbUPC.ptSumVtx->at(i) > MTrackPbPbUPC.ptSumVtx->at(BestVertex)) BestVertex = i;
      }
      if (BestVertex >= 0) {
        MDzeroUPC.VX = MTrackPbPbUPC.xVtx->at(BestVertex);
        MDzeroUPC.VY = MTrackPbPbUPC.yVtx->at(BestVertex);
        MDzeroUPC.VZ = MTrackPbPbUPC.zVtx->at(BestVertex);
        MDzeroUPC.VXError = MTrackPbPbUPC.xErrVtx->at(BestVertex);
        MDzeroUPC.VYError = MTrackPbPbUPC.yErrVtx->at(BestVertex);
        MDzeroUPC.VZError = MTrackPbPbUPC.zErrVtx->at(BestVertex);
      }
      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////
      if (IsData == false){
        for (int iDGen = 0; iDGen < MDzeroGen.Gsize; iDGen++){
          MDzeroUPC.Gpt->push_back(MDzeroGen.Gpt[iDGen]);
          MDzeroUPC.Gy->push_back(MDzeroGen.Gy[iDGen]);
          MDzeroUPC.GpdgId->push_back(MDzeroGen.GpdgId[iDGen]);
          MDzeroUPC.GisSignal->push_back(MDzeroGen.GisSignal[iDGen]);
          MDzeroUPC.GcollisionId->push_back(MDzeroGen.GcollisionId[iDGen]);
          MDzeroUPC.GSignalType->push_back(MDzeroGen.GSignalType[iDGen]);
        }
      }
      if (IsData == true) {
        int pprimaryVertexFilter = MSkim.PVFilter;
        int pclusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;
        int cscTightHalo2015Filter = MFilter.cscTightHalo2015Filter;
        if (pprimaryVertexFilter == 0 || pclusterCompatibilityFilter == 0 || cscTightHalo2015Filter == false) continue;
        if (fabs(MTrackPbPbUPC.zVtx->at(0)) > 15) continue;

        //  FIXME: need to be replaced with the actual PbPb triggers
        int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023         = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000");
        int HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_2023     = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000");
        int HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400");
        int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023  = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000");
        int isL1ZDCOr = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023 || HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023;
        int isL1ZDCXORJet8 = HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023 || HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_2023;
        if (isL1ZDCOr == 0 && isL1ZDCXORJet8 == 0) continue;

        MDzeroUPC.isL1ZDCOr = isL1ZDCOr;
        MDzeroUPC.isL1ZDCXORJet8 = isL1ZDCXORJet8;
        bool ZDCgammaN = (MZDC.sumMinus > 1100. && MZDC.sumPlus < 1100.);
        bool ZDCNgamma = (MZDC.sumMinus < 1100. && MZDC.sumPlus > 1100.);
	
	// Loop through the specified ranges for gapgammaN and gapNgamma
	// gammaN[4] and Ngamma[4] are nominal selection criteria
	bool globalDecision=false;
	for (double gapgammaN_threshold = 5.2; gapgammaN_threshold <= 13.2; gapgammaN_threshold += 1.0) {
            bool gapgammaN = GetMaxEnergyHF(&MPF, 3.0, 5.2) < gapgammaN_threshold;
            bool gammaN_ = ZDCgammaN && gapgammaN;
            MDzeroUPC.gammaN->push_back(gammaN_);
	    if (gammaN_) globalDecision = true;
        }
        for (double gapNgamma_threshold = 4.6; gapNgamma_threshold <= 12.6; gapNgamma_threshold += 1.0) {
            bool gapNgamma = GetMaxEnergyHF(&MPF, -5.2, -3.0) < gapNgamma_threshold;
            bool Ngamma_ = ZDCNgamma && gapNgamma;
            MDzeroUPC.Ngamma->push_back(Ngamma_);
            if (Ngamma_) globalDecision = true;
        }
	if (globalDecision==false) continue;
      } // end of if (IsData == true)

      int nTrackInAcceptanceHP = 0;
      for (int iTrack = 0; iTrack < MTrackPbPbUPC.nTrk; iTrack++) {
        if (MTrackPbPbUPC.trkPt->at(iTrack) <= 0.5) continue; 
	if (fabs(MTrackPbPbUPC.trkEta->at(iTrack)) >= 2.4) continue;
        if (MTrackPbPbUPC.highPurity->at(iTrack) == false) continue;
        nTrackInAcceptanceHP++;
      }
      MDzeroUPC.nTrackInAcceptanceHP = nTrackInAcceptanceHP;

      for (int iD = 0; iD < MDzero.Dsize; iD++) {
        if (MDzero.Dpt[iD] < MinDzeroPT || MDzero.PassUPCDzero2023Cut(iD) == false) continue;
        MDzeroUPC.Dpt->push_back(MDzero.Dpt[iD]);
        MDzeroUPC.Dy->push_back(MDzero.Dy[iD]);
        MDzeroUPC.Dmass->push_back(MDzero.Dmass[iD]);
        MDzeroUPC.Dtrk1Pt->push_back(MDzero.Dtrk1Pt[iD]);
        MDzeroUPC.Dtrk2Pt->push_back(MDzero.Dtrk2Pt[iD]);
        MDzeroUPC.Dchi2cl->push_back(MDzero.Dchi2cl[iD]);
        MDzeroUPC.DsvpvDistance->push_back(MDzero.DsvpvDistance[iD]);
        MDzeroUPC.DsvpvDisErr->push_back(MDzero.DsvpvDisErr[iD]);
        MDzeroUPC.DsvpvDistance_2D->push_back(MDzero.DsvpvDistance_2D[iD]);
        MDzeroUPC.DsvpvDisErr_2D->push_back(MDzero.DsvpvDisErr_2D[iD]);
        MDzeroUPC.Dalpha->push_back(MDzero.Dalpha[iD]);
        MDzeroUPC.Ddtheta->push_back(MDzero.Ddtheta[iD]);
	MDzeroUPC.DpassCut->push_back(DmesonSelection(MDzero,iD));
        if (IsData == false) MDzeroUPC.Dgen->push_back(MDzero.Dgen[iD]);
      }

      MDzeroUPC.FillEntry();
    }

    Bar.Update(EntryCount);
    Bar.Print();
    Bar.PrintLine();

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
  if (M == nullptr)        return -1;
  if (M->Tree == nullptr)  return -1;

  double EMax = 0;
  for (int iPF = 0; iPF < M->ID->size(); iPF++) {
    if ((M->ID->at(iPF) == 6 || M->ID->at(iPF) == 7) && M->Eta->at(iPF) > etaMin && M->Eta->at(iPF) < etaMax) {
      if (M->E->at(iPF) > EMax) EMax = M->E->at(iPF);
    }
  }
  return EMax;
}
