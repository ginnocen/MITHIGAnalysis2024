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
  float ZDCMinus1nThreshold = CL.GetDouble("ZDCMinus1nThreshold", 1000.);
  float ZDCPlus1nThreshold = CL.GetDouble("ZDCPlus1nThreshold", 1100.);
  int ApplyTriggerRejection = CL.GetInteger("ApplyTriggerRejection", 0);
  bool ApplyEventRejection = CL.GetBool("ApplyEventRejection", false);
  bool ApplyZDCGapRejection = CL.GetBool("ApplyZDCGapRejection", false);
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string ZDCTreeName = CL.Get("ZDCTree", "zdcanalyzer/zdcdigi"); // update ZDC info to be rechit when using CMSSW 14_1_X
  string PFJetCollection = CL.Get("PFJetCollection", "ak4PFJetAnalyzer/t"); // original forests: ak4PFJetAnalyzer // new forests: ak0PFJetAnalyzer
  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for D0 Jet UPC Analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  UPCEECTreeMessenger MUPCEEC;
  MUPCEEC.SetBranch(&Tree);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile);
    PbPbUPCTrackTreeMessenger MTrackPbPbUPC(InputFile);
    GenParticleTreeMessenger MGen(InputFile);
    PFTreeMessenger MPF(InputFile, PFTreeName);
    SkimTreeMessenger MSkim(InputFile);
    TriggerTreeMessenger MTrigger(InputFile);
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
      MZDC.GetEntry(iE);
      MUPCEEC.Clear();
      MMETFilter.GetEntry(iE);
      Mjet.GetEntry(iE);



      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MUPCEEC.Run = MEvent.Run;
      MUPCEEC.Lumi = MEvent.Lumi;
      MUPCEEC.Event = MEvent.Event;
      MUPCEEC.Nch = MTrackPbPbUPC.nTrk;

      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////
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
          MUPCEEC.isL1ZDCOr = isL1ZDCOr;
          MUPCEEC.isL1ZDCXORJet8 = isL1ZDCXORJet8;
          MUPCEEC.isL1ZDCXORJet12 = false;
          MUPCEEC.isL1ZDCXORJet16 = false;
          if (ApplyTriggerRejection == 1 && IsData && (isL1ZDCOr == false && isL1ZDCXORJet8 == false)) continue;
          if (ApplyTriggerRejection == 2 && IsData && isL1ZDCOr == false) continue;
        }
        else if (Year == 2024){
          int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v13");
          int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v2");
          int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000 = MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster10000");
          bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000 == 1 || HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000 == 1;
          MUPCEEC.isL1ZDCOr = isL1ZDCOr;
          MUPCEEC.isL1ZDCXORJet8 = false;
          MUPCEEC.isL1ZDCXORJet12 = false;
          MUPCEEC.isL1ZDCXORJet16 = false;
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
        if(gammaN_default)MUPCEEC.isGammaN = true; 
        else MUPCEEC.isGammaN = false; 
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
        if(gapgammaN)MUPCEEC.isGammaN = true; 
        else MUPCEEC.isGammaN = false; 
        if (ApplyZDCGapRejection && gapgammaN == false && gapNgamma == false) continue;


      }

      ////////////////////////////
      ///////// particle flow ////
      ////////////////////////////
      for(int iPF = 0; iPF < MPF.M->size(); iPF++){
        MUPCEEC.E->push_back(MPF.E->at(iPF)); 
        MUPCEEC.PT->push_back(MPF.PT->at(iPF)); 
        MUPCEEC.M->push_back(MPF.M->at(iPF)); 
        MUPCEEC.Eta->push_back(MPF.Eta->at(iPF)); 
        MUPCEEC.Phi->push_back(MPF.Phi->at(iPF));    
      }
      /////////////////////////////

      ////////////////////////////
      ////////// track /////////////
      ////////////////////////////
      int nAccTrk = 0; 
      for (int iTrack = 0; iTrack < MTrackPbPbUPC.nTrk; iTrack++) {
        // quality criteria
        if (MTrackPbPbUPC.highPurity->at(iTrack) == false)continue;
        // position resolution
        double XYVertexSignificance = fabs(MTrackPbPbUPC.trkDxyErrFirstVtx->at(iTrack) / MTrackPbPbUPC.trkDxyFirstVtx->at(iTrack));
        if(XYVertexSignificance >= 3)continue; 
        double ZVertexSignificance = fabs(MTrackPbPbUPC.trkDzErrFirstVtx->at(iTrack) / MTrackPbPbUPC.trkDzFirstVtx->at(iTrack));
        if(ZVertexSignificance >= 3)continue; 
        // Nhits
        if(MTrackPbPbUPC.trkNHits->at(iTrack) < 0) continue; 
        // chi2/ndf/nlayers
        if(MTrackPbPbUPC.trkNormChi2->at(iTrack)/ MTrackPbPbUPC.trkNLayers->at(iTrack) < 0)continue; 
        // eta acceptance
        if(fabs(MTrackPbPbUPC.trkEta->at(iTrack)) > 2.4) continue; 
        //momentum resolution
        if(MTrackPbPbUPC.trkPt->at(iTrack) > 10){
          if(MTrackPbPbUPC.trkPtError->at(iTrack)/MTrackPbPbUPC.trkPt->at(iTrack) > 0.1) continue; 
        }
        else{
          if(MTrackPbPbUPC.trkPtError->at(iTrack)/MTrackPbPbUPC.trkPt->at(iTrack) < 0) continue; 
        }
       
        MUPCEEC.trkPt->push_back(MTrackPbPbUPC.trkPt->at(iTrack));
        MUPCEEC.trkEta->push_back(MTrackPbPbUPC.trkEta->at(iTrack));
        MUPCEEC.trkPhi->push_back(MTrackPbPbUPC.trkPhi->at(iTrack));
        MUPCEEC.pfEnergy->push_back(MTrackPbPbUPC.PFEnergy->at(iTrack)); 

        nAccTrk++;
      }
      MUPCEEC.Nch = nAccTrk; 

      ////////////////////////////
      ////////// Jet /////////////
      ////////////////////////////
      int jetCountPostSelection = 0;
      for (int ijet = 0; ijet < Mjet.JetCount; ijet++) {
        // implement jet pt in cutoff
        if (Mjet.JetPT[ijet] < MinJetPT) continue;

        jetCountPostSelection++;
       // add the basic jet variables to the tree
        MUPCEEC.JetPt->push_back(Mjet.JetPT[ijet]);
        MUPCEEC.JetEta->push_back(Mjet.JetEta[ijet]);
        MUPCEEC.JetY->push_back(Mjet.JetY[ijet]);
        MUPCEEC.JetPhi->push_back(Mjet.JetPhi[ijet]);


      }
      MUPCEEC.JetCount = jetCountPostSelection;

      // now fill the mc only info if we are using that
      if(IsData == false){
        MUPCEEC.pthat = Mjet.PTHat;

        // fill in the info for the gen jets
        for (int igen = 0; igen < Mjet.GenCount; igen++) {
          MUPCEEC.GenJetPt->push_back(Mjet.GenPT[igen]);
          MUPCEEC.GenJetEta->push_back(Mjet.GenEta[igen]);
          MUPCEEC.GenJetY->push_back(Mjet.GenY[igen]);
          MUPCEEC.GenJetPhi->push_back(Mjet.GenPhi[igen]);
        }

            // fill in the info for the matched jets
        for (int ijet = 0; ijet < Mjet.JetCount; ijet++) {
          MUPCEEC.RefJetPt->push_back(Mjet.RefPT[ijet]);
          MUPCEEC.RefJetEta->push_back(Mjet.RefEta[ijet]);
          MUPCEEC.RefJetY->push_back(Mjet.RefY[ijet]);
          MUPCEEC.RefJetPhi->push_back(Mjet.RefPhi[ijet]);

        }

      }



      MUPCEEC.FillEntry();
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
