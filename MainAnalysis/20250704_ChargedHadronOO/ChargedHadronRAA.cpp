#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TTree.h>

#include <iostream>

using namespace std;
#include "CommandLine.h"
#include "Messenger.h"
#include "ProgressBar.h"
#include "helpMessage.h"
#include "parameter.h" // Parameters for the analysis
#include "utilities.h" // Utility functions for the analysis
#include "skimSelectionBits_OO_PP.h"


// Define binnings

const Int_t nPtBins = 47;
const Double_t pTBins_fine[nPtBins + 1] = {
  0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 3.2, 4.0, 4.8, 5.6, 6.4, 7.2, 8.4, 9.6, 10.8, 12.0, 14.4, 15.8, 17.6, 19.2, 22.0, 24.0, 26.2, 28.8, 32.0, 35.2, 38.0, 41.6, 43.6, 48.0, 54.0, 60.8, 73.6, 86.4, 103.6, 120.8, 140.0, 165.0, 250.0, 400.0
};

const Int_t nPtBins_log = 26;
const Double_t pTBins_log[nPtBins_log + 1] = {
  0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31
};

const int nHFBins1 = 30;
const int nHFBins2 = 70;
const int nHFBins = nHFBins1 + nHFBins2;
Double_t HFBins[nHFBins + 1];

bool checkError(const Parameters &par) { return false; }

//============================================================//
// Check if event passes the selection criteria
//============================================================//
bool eventSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, Parameters &par, TH1D* hNEvtPassCuts) {

  hNEvtPassCuts->Fill(1); // Total events

  if (par.IsData && !MChargedHadronRAA->HLT_OxyZeroBias_v1) return false;
  hNEvtPassCuts->Fill(2); // HLT trigger

  if (MChargedHadronRAA->ClusterCompatibilityFilter == false) return false;
  hNEvtPassCuts->Fill(3); // Centrality filter

  if (MChargedHadronRAA->PVFilter == false) return false;
  hNEvtPassCuts->Fill(4); // Primary vertex filter

  if (MChargedHadronRAA->isFakeVtx) return false;
  hNEvtPassCuts->Fill(5); // Not a fake vertex

  // using VZ as found from pTSumVtx method (skim) rather than particle flow method used in the forest
  if (fabs(MChargedHadronRAA->VZ) >= 15.0) return false;
  hNEvtPassCuts->Fill(6); // Vertex Z position within range

  if (MChargedHadronRAA->nTracksVtx < 0) return false;
  hNEvtPassCuts->Fill(7); // Number of tracks in vertex

  if (par.OnlineHFAND > 0 && !checkHFANDCondition(MChargedHadronRAA, par.OnlineHFAND, par.OnlineHFAND, true)) return false;
  hNEvtPassCuts->Fill(8);

  if (par.OfflineHFAND > 0 && !checkHFANDCondition(MChargedHadronRAA, par.OfflineHFAND, par.OfflineHFAND, false)) return false;
  hNEvtPassCuts->Fill(9);

  return true;
}

//============================================================//
// Check if track passes the selection criteria
//============================================================//
bool trackSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, unsigned long j, Parameters &par,TH1D* hNTrkPassCuts) {

  hNTrkPassCuts->Fill(1); // Total tracks considered

  if (j >= MChargedHadronRAA->trkPt->size()) return false;
  hNTrkPassCuts->Fill(2); // Total tracks

  if (fabs(MChargedHadronRAA->trkCharge->at(j)) != 1) return false;
  hNTrkPassCuts->Fill(3); // Charge = 1

  if (MChargedHadronRAA->highPurity->at(j) == false) return false;
  hNTrkPassCuts->Fill(4); // High purity

  if (MChargedHadronRAA->trkPt->at(j) < par.MinTrackPt) return false; // changed from vipuls
  hNTrkPassCuts->Fill(5); // pT > 0.1 GeV/c

  double RelativeUncertainty = MChargedHadronRAA->trkPtError->at(j) / MChargedHadronRAA->trkPt->at(j);
  if (MChargedHadronRAA->trkPt->at(j) > 10 && RelativeUncertainty > 0.1) return false;
  hNTrkPassCuts->Fill(6); // Relative uncertainty < 10%

  if (fabs(MChargedHadronRAA->trkDxyAssociatedVtx->at(j)) / MChargedHadronRAA->trkDxyErrAssociatedVtx->at(j) > 3) return false;
  hNTrkPassCuts->Fill(7); // Dxy < 3 sigma

  if(fabs(MChargedHadronRAA->trkDzAssociatedVtx->at(j)) / MChargedHadronRAA->trkDzErrAssociatedVtx->at(j) > 3) return false;
  hNTrkPassCuts->Fill(8); // Dz < 3 sigma

  if (MChargedHadronRAA->trkPt->at(j) > par.MaxTrackPt) return false;
  hNTrkPassCuts->Fill(9); // pT < 500 GeV/c

  return true;
}


//============================================================//
// Data analyzer class
//============================================================//
class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt, *hTrkEta, *hMult, *hhiHF_pf;
  TH2D *hTrkPtEta, *hHFEMaxPlusMinus, *hhiHFPlusMinus_pf, *hZDCPlusMinus;
  TH1D *hNEvtPassCuts, *hNTrkPassCuts;
  TH3D *hVXYZ;
  TH1D *hVZ_pf;
  ChargedHadronRAATreeMessenger *MChargedHadronRAA;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), 
        MChargedHadronRAA(new ChargedHadronRAATreeMessenger(*inf, string("Tree"), false, false, 2)),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MChargedHadronRAA;
  }

  void analyze(Parameters &par) {
    outf->cd();

    // Fill first 30 bins: linearly spaced between 0 and 15
    for (int i = 0; i <= nHFBins1; ++i) {
      HFBins[i] = 0.0 + (15.0 - 0.0) * i / nHFBins1;
    }
    // Fill next 70 bins: linearly spaced between 15 and 600
    for (int i = 1; i <= nHFBins2; ++i) {
      HFBins[nHFBins1 + i] = 15.0 + (600.0 - 15.0) * i / nHFBins2;
    }

    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEta = new TH1D(Form("hTrkEta%s", title.c_str()), "", 50, -3.0, 3.0);
    hMult = new TH1D(Form("hMult%s", title.c_str()), "Multiplicity", 100, 0.0, 1000);
    hhiHF_pf = new TH1D(Form("hhiHF_pf%s", title.c_str()), "HF ET Sum", 100, 0.0, 2000);
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", nPtBins, pTBins_fine, 50, -4.0, 4.0);
    hHFEMaxPlusMinus = new TH2D(Form("hHFEMaxPlusMinus%s", title.c_str()), "HF E Max Plus, Minus", nHFBins, HFBins, nHFBins, HFBins);
    hhiHFPlusMinus_pf = new TH2D(Form("hhiHFPlusMinus_pf%s", title.c_str()), "HF ET Sum Plus, Minus", 100, 0, 400, 100, 0, 400);
    hZDCPlusMinus = new TH2D(Form("hZDCPlusMinus%s", title.c_str()), "ZDC Plus, Minus Energy", 100, 0.0, 10000, 100, 0.0, 10000);
    hVXYZ = new TH3D(Form("hVXYZ%s", title.c_str()), "Vertex XYZ position", 100, -30.0, 30.0, 100, -30.0, 30.0, 100, -30.0, 30.0);
    hVZ_pf = new TH1D(Form("hVZ_pf%s", title.c_str()), "Vertex Z position (PF)", 100, -30.0, 30.0);

    hTrkPt->Sumw2();
    hTrkEta->Sumw2();
    hTrkPtEta->Sumw2();
    hMult->Sumw2();
    hhiHF_pf->Sumw2();
    hHFEMaxPlusMinus->Sumw2();
    hhiHFPlusMinus_pf->Sumw2();
    hZDCPlusMinus->Sumw2();
    hVXYZ->Sumw2();
    hVZ_pf->Sumw2();

    hNEvtPassCuts = new TH1D("hNEvtPassCuts", "Number of events passing cuts", 9, 0.5, 9.5);
    hNEvtPassCuts->GetXaxis()->SetBinLabel(1, "Total Events");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(2, "+ HLT_OxyZeroBias_v1");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(3, "+ CC");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(4, "+ PV");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(5, "+ !isFakeVtx");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(6, "+ abs(VZ)<15");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(7, "+ nTrk>=0");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(8, "+ Online HF AND 14");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(9, "+ Offline HF AND 12");
    hNEvtPassCuts->Sumw2();

    hNTrkPassCuts = new TH1D("hNTrkPassCuts", "Number of tracks passing cuts", 9, 0.5, 9.5);
    hNTrkPassCuts->GetXaxis()->SetBinLabel(1, "Total Tracks");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(2, "+ nTrk > 0");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(3, "+ abs(charge)=1");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(4, "+ High Purity");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(5, "+ pT > 0. GeV/c");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(6, "+ pT > 10 && Rel pT Error < 10%");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(7, "+ Dxy < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(8, "+ Dz < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(9, "+ pT < 500 GeV/c");
    hNTrkPassCuts->Sumw2();

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    int eventsRejected = 0;
    // event loop
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // get event selection efficiency correction, and diffraction contamination correction
      float eventWeight = 1.0;
      if (par.UseEventWeight) {
        eventWeight *= MChargedHadronRAA->eventWeight;
      }

      // event selection criteria
      if (par.ApplyEventSelection && !eventSelection(MChargedHadronRAA, par,hNEvtPassCuts)) {
        eventsRejected++;
        continue;
      }

      // event-level histograms
      hMult->Fill(MChargedHadronRAA->multipicityEta2p4, eventWeight);
      hhiHF_pf->Fill(MChargedHadronRAA->hiHF_pf, eventWeight);
      hHFEMaxPlusMinus->Fill(MChargedHadronRAA->HFEMaxPlus, MChargedHadronRAA->HFEMaxMinus, eventWeight);
      hhiHFPlusMinus_pf->Fill(MChargedHadronRAA->hiHFPlus_pf, MChargedHadronRAA->hiHFMinus_pf, eventWeight);
      if (par.IsData) hZDCPlusMinus->Fill(MChargedHadronRAA->ZDCsumPlus, MChargedHadronRAA->ZDCsumMinus, eventWeight); // no ZDC in MC
      hVXYZ->Fill(MChargedHadronRAA->VX, MChargedHadronRAA->VY, MChargedHadronRAA->VZ, eventWeight);
      hVZ_pf->Fill(MChargedHadronRAA->VZ_pf, eventWeight);

      // track loop
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {

        // get track selection efficiency correction
        float trkWeight = 1.0;
        if (par.UseTrackWeight) {
          trkWeight *= MChargedHadronRAA->trackWeight->at(j);
        }
        float eventTrkWeight = eventWeight * trkWeight;

        // track selection w/o eta cut
        if (!trackSelection(MChargedHadronRAA, j, par, hNTrkPassCuts)) continue;

        // eta hist before applying eta cut
        hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), eventTrkWeight);
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j), eventTrkWeight);

        // apply eta cut (last track selection)
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0) continue;

        // fill dN/dpT
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), eventTrkWeight);

      } // end of track loop
    } // end of event loop

    cout << endl;
    cout << "Total events: " << nEntry << endl;
    cout << "Events rejected: " << eventsRejected << endl;
    cout << "Events passing cuts: " << nEntry - eventsRejected << endl;

  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hTrkPt);
    smartWrite(hTrkEta);
    smartWrite(hMult);
    smartWrite(hhiHF_pf);
    smartWrite(hTrkPtEta);
    smartWrite(hHFEMaxPlusMinus);
    smartWrite(hhiHFPlusMinus_pf);
    smartWrite(hZDCPlusMinus);
    smartWrite(hVXYZ);
    smartWrite(hVZ_pf);
    smartWrite(hNEvtPassCuts);
    smartWrite(hNTrkPassCuts);
  }

private:
  void deleteHistograms() {
    delete hTrkPt;
    delete hTrkEta;
    delete hMult;
    delete hhiHF_pf;
    delete hTrkPtEta;
    delete hHFEMaxPlusMinus;
    delete hhiHFPlusMinus_pf;
    delete hZDCPlusMinus;
    delete hVXYZ;
    delete hVZ_pf;
    delete hNEvtPassCuts;
    delete hNTrkPassCuts;
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  
  if (printHelpMessage(argc, argv)) return 0;

  CommandLine CL(argc, argv);
  float MinTrackPt  = CL.GetDouble("MinTrackPt", 0.1);      // Minimum track transverse momentum threshold for track selection.
  bool IsData       = CL.GetBool("IsData", true);              // Determines whether the analysis is being run on actual data.
  int TriggerChoice = CL.GetInt("TriggerChoice", 0);        // Flag indication choice of trigger
  float scaleFactor = CL.GetDouble("ScaleFactor", 1.0);     // Fraction of the total number of events to be processed

  Parameters par(MinTrackPt, TriggerChoice, IsData, scaleFactor);
  par.input           = CL.Get("Input", "input.root");        // Input file
  par.output          = CL.Get("Output", "output.root");      // Output file
  par.IsPP            = CL.GetBool("IsPP", false);            // Flag to indicate if the analysis is for Proton-Proton collisions.
  par.MaxTrackPt      = CL.GetDouble("MaxTrackPt", 500.0);    // Maximum track transverse momentum threshold for track selection.
  par.UseTrackWeight  = CL.GetBool("UseTrackWeight", true);
  par.UseEventWeight  = CL.GetBool("UseEventWeight", true);
  par.ApplyEventSelection = CL.GetBool("ApplyEventSelection", true);
  par.OnlineHFAND     = CL.GetDouble("OnlineHFAND", -1);    // Online HF AND condition, -1 cut not applied
  par.OfflineHFAND    = CL.GetDouble("OfflineHFAND", -1);    // Offline HF AND condition, -1 cut not applied

  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;

  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;

  return 0;
}
