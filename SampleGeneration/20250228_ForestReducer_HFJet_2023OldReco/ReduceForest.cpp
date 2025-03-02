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

bool logical_or_vectBool(std::vector<bool>* vec) {
    return std::any_of(vec->begin(), vec->end(), [](bool b) { return b; });
}

int main(int argc, char *argv[]);
double GetMaxEnergyHF(PFTreeMessenger *M, double etaMin, double etaMax);

int main(int argc, char *argv[]) {
  string VersionString = "V0";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  // bool DoGenLevel                    = CL.GetBool("DoGenLevel", true);
  bool IsData = CL.GetBool("IsData", false);
  int Year = CL.GetInt("Year", 2023);
  double Fraction = CL.GetDouble("Fraction", 1.00);
  double MinJetPT = CL.GetDouble("MinJetPT", 0.0);
  double MaxJetEta = CL.GetDouble("MaxJetEta", 2.4);
  double D0MathchingDistance = CL.GetDouble("D0MathchingDistance", 0.4);
  float ZDCMinus1nThreshold = CL.GetDouble("ZDCMinus1nThreshold", 1000.);
  float ZDCPlus1nThreshold = CL.GetDouble("ZDCPlus1nThreshold", 1100.);
  int ApplyTriggerRejection = CL.GetInteger("ApplyTriggerRejection", 0);
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  bool ApplyZDCGapRejection = CL.GetBool("ApplyZDCGapRejection", false);
  int ApplyDRejection = CL.GetInteger("ApplyDRejection", 0);
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string DGenTreeName = CL.Get("DGenTree", "Dfinder/ntGen");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcdigi"); // update ZDC info to be rechit when using CMSSW 14_1_X
  string PFJetCollection = CL.Get("PFJetCollection", "ak4PFJetAnalyzer/t"); // original forests: ak4PFJetAnalyzer // new forests: ak0PFJetAnalyzer
  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for D0 Jet UPC Analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  DzeroJetUPCTreeMessenger MDzeroJetUPC;
  MDzeroJetUPC.SetBranch(&Tree);

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
    ZDCTreeMessenger MZDC(InputFile, ZDCTreeName);
    METFilterTreeMessenger MMETFilter(InputFile);
    JetTreeMessenger Mjet(InputFile, PFJetCollection);



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
      if (IsData == false)
        MDzeroGen.GetEntry(iE);
      MZDC.GetEntry(iE);
      MDzeroJetUPC.Clear();
      MMETFilter.GetEntry(iE);
      Mjet.GetEntry(iE);



      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MDzeroJetUPC.Run = MEvent.Run;
      MDzeroJetUPC.Lumi = MEvent.Lumi;
      MDzeroJetUPC.Event = MEvent.Event;
      MDzeroJetUPC.Nch = MTrackPbPbUPC.nTrk;

      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////
      if (IsData == false) {
         MDzeroJetUPC.Gsize = MDzeroGen.Gsize;
        for (int iDGen = 0; iDGen < MDzeroGen.Gsize; iDGen++) {
          MDzeroJetUPC.Gpt->push_back(MDzeroGen.Gpt[iDGen]);
          MDzeroJetUPC.Gy->push_back(MDzeroGen.Gy[iDGen]);
          MDzeroJetUPC.Gphi->push_back(MDzeroGen.Gphi[iDGen]);
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
          MDzeroJetUPC.isL1ZDCOr = isL1ZDCOr;
          MDzeroJetUPC.isL1ZDCXORJet8 = isL1ZDCXORJet8;
          MDzeroJetUPC.isL1ZDCXORJet12 = false;
          MDzeroJetUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData && (isL1ZDCOr == false && isL1ZDCXORJet8 == false)) continue;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2024){
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13");
          int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2");
          int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000");
          bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 == 1 || HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 == 1;
          MDzeroJetUPC.isL1ZDCOr = isL1ZDCOr;
          MDzeroJetUPC.isL1ZDCXORJet8 = false;
          MDzeroJetUPC.isL1ZDCXORJet12 = false;
          MDzeroJetUPC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData) std::cout << "Trigger rejection ZDCOR || ZDCXORJet8 not implemented for 2024" << std::endl;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
     }
     if (IsData == true) {
        // Use the same event selection as the HIN-24-003
        // apply the event selection but do not save it.
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1 && MMETFilter.cscTightHalo2015Filter;
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        // apply basic event rejection
        if (ApplyEventRejection && IsData && (selectedBkgFilter == false || selectedVtxFilter == false)) continue;
        bool ZDCgammaN = (MZDC.sumMinus > ZDCMinus1nThreshold && MZDC.sumPlus < ZDCPlus1nThreshold);
        bool ZDCNgamma = (MZDC.sumMinus < ZDCMinus1nThreshold && MZDC.sumPlus > ZDCPlus1nThreshold);
        float EMaxHFPlus = GetMaxEnergyHF(&MPF, 3., 5.2);
        float EMaxHFMinus = GetMaxEnergyHF(&MPF, -5.2, -3.);
        bool gapgammaN = EMaxHFPlus < 9.2;
        bool gapNgamma = EMaxHFMinus < 8.6;
        bool gammaN_default = ZDCgammaN && gapgammaN;
        bool Ngamma_default = ZDCNgamma && gapNgamma;
        // apply rapidity gap rejection
        if (ApplyZDCGapRejection && IsData && gammaN_default == false && Ngamma_default == false) continue;

      } // end of if (IsData == true)

      // for the monte carlo want to apply the event selections as well
      if(!IsData){
        bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1;
        bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15.;
        if (ApplyEventRejection && (selectedBkgFilter == false || selectedVtxFilter == false))continue;
        float EMaxHFPlus = GetMaxEnergyHF(&MPF, 3., 5.2);
        float EMaxHFMinus = GetMaxEnergyHF(&MPF, -5.2, -3.);
        bool gapgammaN = EMaxHFPlus < 9.2;
        bool gapNgamma = EMaxHFMinus < 8.6;
        if (ApplyZDCGapRejection && gapgammaN == false && gapNgamma == false) continue;


      }


      ////////////////////////////
      ////////// Dzero /////////////
      ////////////////////////////
      int countSelDzero = 0;
      for (int iD = 0; iD < MDzero.Dsize; iD++) {
        if (ApplyDRejection == 1 && DmesonSelectionSkimInUPCJet(MDzero, iD) == false) continue;
        countSelDzero++;
        MDzeroJetUPC.Dpt->push_back(MDzero.Dpt[iD]);
        MDzeroJetUPC.Dy->push_back(MDzero.Dy[iD]);
        MDzeroJetUPC.Dphi->push_back(MDzero.Dphi[iD]);
        MDzeroJetUPC.Dmass->push_back(MDzero.Dmass[iD]);
        MDzeroJetUPC.DpassCutD0inJet->push_back(DmesonSelectionSkimInUPCJet(MDzero,iD));
        if (IsData == false) {
          MDzeroJetUPC.Dgen->push_back(MDzero.Dgen[iD]);
        }
      }
      MDzeroJetUPC.Dsize = countSelDzero;

      ////////////////////////////
      ////////// Jet /////////////
      ////////////////////////////
      int jetCountPostSelection = 0;
      for (int ijet = 0; ijet < Mjet.JetCount; ijet++) {
        // implement jet pt in cutoff
        if (Mjet.JetPT[ijet] < MinJetPT) continue;

        jetCountPostSelection++;
       // add the basic jet variables to the tree
        MDzeroJetUPC.JetPt->push_back(Mjet.JetPT[ijet]);
        MDzeroJetUPC.JetEta->push_back(Mjet.JetEta[ijet]);
        MDzeroJetUPC.JetY->push_back(Mjet.JetY[ijet]);
        MDzeroJetUPC.JetPhi->push_back(Mjet.JetPhi[ijet]);

        // now write some code to see if the jet can be called D0 tagged
        // note this tagging is before any pT cut of the D0
        // for each jet check if there is a d0
        // only check the selected d0s
        double minDistance = 9999;
        int d0Index = -1;
        for(int id = 0; id < MDzeroJetUPC.Dpt->size(); id++){
            // calculate the distance
            double deltaYsq = (Mjet.JetY[ijet] - MDzeroJetUPC.Dy->at(id)) *  (Mjet.JetY[ijet] - MDzeroJetUPC.Dy->at(id));
            double deltaPhisq = (Mjet.JetPhi[ijet] - MDzeroJetUPC.Dphi->at(id)) * (Mjet.JetPhi[ijet] - MDzeroJetUPC.Dphi->at(id));
            double dist = sqrt(deltaYsq + deltaPhisq);
            if(dist < minDistance) {
              minDistance = dist;
              d0Index = id;
	      // if we found one, no need to continue
	      break;
            }
        } // end of Dzero loop

          // if after looping over all of the D0s we found one within the cone, fill the jet pt
         if(minDistance < D0MathchingDistance){
          MDzeroJetUPC.isD0TaggedGeomJet->push_back(true);
          MDzeroJetUPC.TaggedLeadingD0GeomInJetIndex->push_back(d0Index);
         }
         else{
          MDzeroJetUPC.isD0TaggedGeomJet->push_back(false);
          MDzeroJetUPC.TaggedLeadingD0GeomInJetIndex->push_back(-1);
         }


      }
      MDzeroJetUPC.JetCount = jetCountPostSelection;

      // now fill the mc only info if we are using that
      if(IsData == false){
        MDzeroJetUPC.pthat = Mjet.PTHat;

        // fill in the info for the gen jets
        for (int igen = 0; igen < Mjet.GenCount; igen++) {
          MDzeroJetUPC.GenJetPt->push_back(Mjet.GenPT[igen]);
          MDzeroJetUPC.GenJetEta->push_back(Mjet.GenEta[igen]);
          MDzeroJetUPC.GenJetY->push_back(Mjet.GenY[igen]);
          MDzeroJetUPC.GenJetPhi->push_back(Mjet.GenPhi[igen]);
        }

            // fill in the info for the matched jets
        for (int ijet = 0; ijet < Mjet.JetCount; ijet++) {
          MDzeroJetUPC.RefJetPt->push_back(Mjet.RefPT[ijet]);
          MDzeroJetUPC.RefJetEta->push_back(Mjet.RefEta[ijet]);
          MDzeroJetUPC.RefJetY->push_back(Mjet.RefY[ijet]);
          MDzeroJetUPC.RefJetPhi->push_back(Mjet.RefPhi[ijet]);

        }

      }



      MDzeroJetUPC.FillEntry();
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
