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

#include "eventSelectionCorrection.h"

// Define binnings

const Int_t nPtBins = 37;
const Double_t pTBins_fine[nPtBins+1] = {
  0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,3.2,4,4.8,5.6,6.4,7.2,9.6,12,14.4,19.2,24,28.8,35.2,41.6,48,60.8,73.6,86.4,103.6,120.8,140,165,250,400
};

const Int_t nPtBins_log = 26;
const Double_t pTBins_log[nPtBins_log + 1] = {
  0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31
};

bool checkError(const Parameters &par) { return false; }

//============================================================//
// Check if event passes the selection criteria
//============================================================//
bool eventSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, Parameters &par, TH1D* hNEvtPassCuts) {

  hNEvtPassCuts->Fill(1); // Total events

  if (par.MinLeadingTrackPt > 0.0 && MChargedHadronRAA->leadingPtEta1p0_sel < par.MinLeadingTrackPt) return false;
  hNEvtPassCuts->Fill(2); // Leading track pT > MinLeadingTrackPt

  //if (MChargedHadronRAA->ClusterCompatibilityFilter == false) return false;
  //hNEvtPassCuts->Fill(3); // Centrality filter

  if (MChargedHadronRAA->PVFilter == false) return false;
  hNEvtPassCuts->Fill(3); // Primary vertex filter  

  //if (MChargedHadronRAA->isFakeVtx) return false;
  //hNEvtPassCuts->Fill(5); // Not a fake vertex

  //if (MChargedHadronRAA->nTracksVtx < 0) return false;
  //hNEvtPassCuts->Fill(6); // Number of tracks in vertex

  // using VZ as found from pTSumVtx method (skim) rather than particle flow method used in the forest
  if (fabs(MChargedHadronRAA->VZ_pf) >= 15.0) return false;
  hNEvtPassCuts->Fill(4); // Vertex Z position within range

  return true;
}

//============================================================//
// Check if track passes the selection criteria
//============================================================//
bool trackSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, unsigned long j, Parameters &par,TH1D* hNTrkPassCuts) {

  //hNTrkPassCuts->Fill(1); // Total tracks considered

  if (j >= MChargedHadronRAA->trkPt->size()) return false;
  //hNTrkPassCuts->Fill(2); // Total tracks

  if (fabs(MChargedHadronRAA->trkCharge->at(j)) != 1) return false;
  //hNTrkPassCuts->Fill(3); // Charge = 1

  if (MChargedHadronRAA->highPurity->at(j) == false) return false;
  //hNTrkPassCuts->Fill(4); // High purity

  if (MChargedHadronRAA->trkPt->at(j) < par.MinTrackPt) return false; // changed from vipuls
  //hNTrkPassCuts->Fill(5); // pT > 0.1 GeV/c

  double RelativeUncertainty = MChargedHadronRAA->trkPtError->at(j) / MChargedHadronRAA->trkPt->at(j);
  if (MChargedHadronRAA->trkPt->at(j) > 10 && RelativeUncertainty > 0.1) return false;
  //hNTrkPassCuts->Fill(6); // Relative uncertainty < 10%

  if (fabs(MChargedHadronRAA->trkDxyAssociatedVtx->at(j)) / MChargedHadronRAA->trkDxyErrAssociatedVtx->at(j) > 3) return false;
  //hNTrkPassCuts->Fill(7); // Dxy < 3 sigma

  if(fabs(MChargedHadronRAA->trkDzAssociatedVtx->at(j)) / MChargedHadronRAA->trkDzErrAssociatedVtx->at(j) > 3) return false;
  //hNTrkPassCuts->Fill(8); // Dz < 3 sigma

  if (MChargedHadronRAA->trkPt->at(j) > par.MaxTrackPt) return false;
  //hNTrkPassCuts->Fill(9); // pT < 500 GeV/c

  return true;
}

int getMultiplicity(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, Parameters &par, float eta) {

  int mult = 0;
  
  for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
    if (trackSelection(MChargedHadronRAA, j, par, nullptr) && 
        fabs(MChargedHadronRAA->trkEta->at(j)) < eta) {
      mult++;
    }
  }

  return mult;
}

float getEventCorrection(ChargedHadronRAATreeMessenger *MChargedHadronRAA, TH1D* eventSelectionEfficiency) {

  float mult = MChargedHadronRAA->multiplicityEta2p4;

  int bin = eventSelectionEfficiency->FindBin(mult);
  if (mult < eventSelectionEfficiency->GetXaxis()->GetXmin() ||
      mult > eventSelectionEfficiency->GetXaxis()->GetXmax())
    return 1.0;

  double eff = eventSelectionEfficiency->GetBinContent(bin);
  if (eff <= 0)
    return 1.0;

  return 1.0 / eff;

}


//============================================================//
// Data analyzer class
//============================================================//
class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt, *hTrkEta;
  TH1D *hNEvtPassCuts, *hNTrkPassCuts, *hTrkWeight;
  TH2D *hTrkWeightPt, *hTrkWeightEta;
  TH2D *hTrkPtEta;
  TH3D *hVXYZ;
  TH1D *hVZ_pf, *hTrkPt_noSel;
  TH1D *hLeadingTrkPt, *hLeadingTrkPt_noSel, *hMult, *hMult_noSel, *hMult_oneVtx, *hMult_noSel_oneVtx, *hMult_oneVtx_Eta1p5, *hMult_noSel_oneVtx_Eta1p5;
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

    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPt_noSel = new TH1D(Form("hTrkPt_noSel%s", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEta = new TH1D(Form("hTrkEta%s", title.c_str()), "", 50, -3.0, 3.0);
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", nPtBins, pTBins_fine, 50, -4.0, 4.0);
    hVXYZ = new TH3D(Form("hVXYZ%s", title.c_str()), "Vertex XYZ position", 100, -30.0, 30.0, 100, -30.0, 30.0, 100, -30.0, 30.0);
    hVZ_pf = new TH1D(Form("hVZ_pf%s", title.c_str()), "Vertex Z position (PF)", 100, -30.0, 30.0);
    hTrkWeight = new TH1D(Form("hTrkWeight%s", title.c_str()), "Track Weight", 100, 1, 1.7);
    hTrkWeightPt = new TH2D(Form("hTrkWeightPt%s", title.c_str()), "Track Weight vs pT", nPtBins_log, pTBins_log, 100, 1, 1.7);
    hTrkWeightEta = new TH2D(Form("hTrkWeightEta%s", title.c_str()), "Track Weight vs #eta", 50, -3.0, 3.0, 100, 1, 1.7);
    hLeadingTrkPt = new TH1D(Form("hLeadingTrkPt%s", title.c_str()), "Leading Track pT", nPtBins_log, pTBins_log);
    hLeadingTrkPt_noSel = new TH1D(Form("hLeadingTrkPt_noSel%s", title.c_str()), "Leading Track pT (no selection)", nPtBins_log, pTBins_log);
    hMult = new TH1D(Form("hMult%s", title.c_str()), "Multiplicity", 200, 0, 200);
    hMult_noSel = new TH1D(Form("hMult_noSel%s", title.c_str()), "Multiplicity (no selection)", 200, 0, 200);
    hMult_oneVtx = new TH1D(Form("hMult_oneVtx%s", title.c_str()), "Multiplicity (one vertex)", 200, 0, 200);
    hMult_oneVtx_Eta1p5 = new TH1D(Form("hMult_oneVtx_Eta1p5%s", title.c_str()), "Multiplicity (one vertex, |#eta| < 1.0)", 200, 0, 200);
    hMult_noSel_oneVtx = new TH1D(Form("hMult_noSel_oneVtx%s", title.c_str()), "Multiplicity (no selection, one vertex)", 200, 0, 200);
    hMult_noSel_oneVtx_Eta1p5 = new TH1D(Form("hMult_noSel_oneVtx_Eta1p5%s", title.c_str()), "Multiplicity (no selection, one vertex, |#eta| < 1.0)", 200, 0, 200);

    hTrkPt->Sumw2();
    hTrkEta->Sumw2();
    hTrkPtEta->Sumw2();
    hMult->Sumw2();
    hMult_noSel->Sumw2();
    hMult_oneVtx->Sumw2();
    hMult_noSel_oneVtx->Sumw2();
    hMult_oneVtx_Eta1p5->Sumw2();
    hMult_noSel_oneVtx_Eta1p5->Sumw2();
    hVXYZ->Sumw2();
    hVZ_pf->Sumw2();
    hTrkWeight->Sumw2();
    hTrkWeightPt->Sumw2();
    hTrkWeightEta->Sumw2();
    hLeadingTrkPt->Sumw2();
    hLeadingTrkPt_noSel->Sumw2();

    hNEvtPassCuts = new TH1D("hNEvtPassCuts", "Number of events passing cuts", 4, 0.5, 4.5);
    hNEvtPassCuts->GetXaxis()->SetBinLabel(1, "Total Events");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(2, "+ min leading track pT");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(3, "+ Primary Vertex Filter");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(4, "+ abs(VZ)<15");
    hNEvtPassCuts->Sumw2();

    hNTrkPassCuts = new TH1D("hNTrkPassCuts", "Number of tracks passing cuts", 9, 0.5, 9.5);
    hNTrkPassCuts->GetXaxis()->SetBinLabel(1, "Total Tracks");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(2, "+ nTrk > 0");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(3, "+ abs(charge)=1");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(4, "+ High Purity");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(5, "+ pT > 0.4 GeV/c");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(6, "+ pT > 10 && Rel pT Error < 10%");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(7, "+ Dxy < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(8, "+ Dz < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(9, "+ pT < 500 GeV/c");
    hNTrkPassCuts->Sumw2();

    // load event selection efficiency histogram
    TH1D* eventSelectionEfficiency = nullptr;
    if (par.UseEventWeight && par.EventCorrectionFile != "") {
      TFile *eventEffFile = TFile::Open(par.EventCorrectionFile.c_str());
      eventSelectionEfficiency = (TH1D*)eventEffFile->Get("hEff");
    }

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    int eventsRejected = 0;
    // event loop
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (!par.HideProgressBar && i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // get event selection efficiency correction
      float eventWeight = 1.0;
      if (par.UseEventWeight) {
        //eventWeight *= MChargedHadronRAA->eventWeight;
        eventWeight *= getEventCorrection(MChargedHadronRAA, eventSelectionEfficiency);
      }
      //cout<< "Event weight: " << eventWeight << endl;

      // calculate multiplicity for |eta| < 1.5 by hand since skim doesnt have it
      int multiplicityEta1p5 = getMultiplicity(MChargedHadronRAA, par, 1.5);

      // fill without event selection
      hLeadingTrkPt_noSel->Fill(MChargedHadronRAA->leadingPtEta1p0_sel);
      hMult_noSel->Fill(MChargedHadronRAA->multiplicityEta2p4);
      if (MChargedHadronRAA->nVtx == 1 && !MChargedHadronRAA->isFakeVtx) {
        hMult_noSel_oneVtx_Eta1p5->Fill(multiplicityEta1p5);
        hMult_noSel_oneVtx->Fill(MChargedHadronRAA->multiplicityEta2p4);
      }

      // track loop, no evt selection
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0) continue;
        hTrkPt_noSel->Fill(MChargedHadronRAA->trkPt->at(j));
      }

      // event selection criteria
      if (par.ApplyEventSelection && !eventSelection(MChargedHadronRAA, par,hNEvtPassCuts)) {
        eventsRejected++;
        continue;
      }

      // event-level histograms
      hVXYZ->Fill(MChargedHadronRAA->VX, MChargedHadronRAA->VY, MChargedHadronRAA->VZ, eventWeight);
      hVZ_pf->Fill(MChargedHadronRAA->VZ_pf, eventWeight);
      hLeadingTrkPt->Fill(MChargedHadronRAA->leadingPtEta1p0_sel);
      hMult->Fill(MChargedHadronRAA->multiplicityEta2p4);
      if (MChargedHadronRAA->nVtx == 1 && !MChargedHadronRAA->isFakeVtx) {
        hMult_oneVtx_Eta1p5->Fill(multiplicityEta1p5);
        hMult_oneVtx->Fill(MChargedHadronRAA->multiplicityEta2p4);
      }

      // track loop
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {

        // get track selection efficiency correction
        float trkWeight = 1.0;
        if (par.UseTrackWeight) {
          if (par.TrackWeightSelection == 1) trkWeight *= MChargedHadronRAA->trackingEfficiency_Loose->at(j);
          else if (par.TrackWeightSelection == 2) trkWeight *= MChargedHadronRAA->trackingEfficiency_Nominal->at(j);
          else if (par.TrackWeightSelection == 3) trkWeight *= MChargedHadronRAA->trackingEfficiency_Tight->at(j);
          else if (par.TrackWeightSelection == 4) trkWeight *= MChargedHadronRAA->trackingEfficiency2017pp->at(j);
        }
        float eventTrkWeight = eventWeight * trkWeight;
        //cout<<eventTrkWeight << endl;

        // track selection w/o eta cut
        if (!trackSelection(MChargedHadronRAA, j, par, hNTrkPassCuts)) continue;

        // eta hist before applying eta cut
        hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), eventTrkWeight);

        // apply eta cut (last track selection)
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0) continue;

        // fill dN/dpT
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), eventTrkWeight);
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j), eventTrkWeight);
        hTrkWeightPt->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight);
        hTrkWeightEta->Fill(MChargedHadronRAA->trkEta->at(j), trkWeight);
        hTrkWeight->Fill(eventTrkWeight);

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
    smartWrite(hMult_noSel);
    smartWrite(hMult_oneVtx);
    smartWrite(hMult_noSel_oneVtx);
    smartWrite(hMult_oneVtx_Eta1p5);
    smartWrite(hMult_noSel_oneVtx_Eta1p5);
    smartWrite(hTrkPtEta);
    smartWrite(hTrkWeight);
    smartWrite(hVXYZ);
    smartWrite(hVZ_pf);
    smartWrite(hNEvtPassCuts);
    smartWrite(hNTrkPassCuts);
    smartWrite(hTrkWeightPt);
    smartWrite(hTrkWeightEta);
    smartWrite(hLeadingTrkPt);
    smartWrite(hLeadingTrkPt_noSel);
    smartWrite(hTrkPt_noSel);
  }

private:
  void deleteHistograms() {
    delete hTrkPt;
    delete hTrkEta;
    delete hMult;
    delete hMult_noSel;
    delete hMult_oneVtx;
    delete hMult_noSel_oneVtx;
    delete hMult_oneVtx_Eta1p5;
    delete hMult_noSel_oneVtx_Eta1p5;
    delete hTrkPtEta;
    delete hTrkWeight;
    delete hVXYZ;
    delete hVZ_pf;
    delete hNEvtPassCuts;
    delete hNTrkPassCuts;
    delete hTrkWeightPt;
    delete hTrkWeightEta;
    delete hLeadingTrkPt;
    delete hLeadingTrkPt_noSel;
    delete hTrkPt_noSel;
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
  par.MinLeadingTrackPt = CL.GetDouble("MinLeadingTrackPt", -1); // Minimum leading track pT for event selection
  par.TrackWeightSelection = CL.GetInt("TrackWeightSelection", 1); // Selection criteria for track weight
  // Removed HF flags
  // par.OnlineHFAND     = CL.GetDouble("OnlineHFAND", -1);
  // par.OfflineHFAND    = CL.GetDouble("OfflineHFAND", -1);
  par.EventCorrectionFile = CL.Get("EventCorrectionFile", ""); // File containing event correction factors
  par.HideProgressBar = CL.GetBool("HideProgressBar", false);

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
