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

//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters &par) { return false; }


bool eventSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, TH1D* hNEvtPassCuts) {

  hNEvtPassCuts->Fill(1); // Total events

  if (MChargedHadronRAA->ClusterCompatibilityFilter == false) return false;
  hNEvtPassCuts->Fill(2); // Centrality filter

  if (MChargedHadronRAA->PVFilter == false) return false;
  hNEvtPassCuts->Fill(3); // Primary vertex filter

  if (MChargedHadronRAA->isFakeVtx) return false;
  hNEvtPassCuts->Fill(4); // Not a fake vertex

  if (fabs(MChargedHadronRAA->VZ) >= 15.0) return false;
  hNEvtPassCuts->Fill(5); // Vertex Z position within range

  if (MChargedHadronRAA->nTracksVtx < 0) return false;
  hNEvtPassCuts->Fill(6); // Number of tracks in vertex

  return true;
}

bool trackSelection(const ChargedHadronRAATreeMessenger *MChargedHadronRAA, unsigned long j, TH1D* hNTrkPassCuts) {

  if (j >= MChargedHadronRAA->trkPt->size()) return false;

  hNTrkPassCuts->Fill(1); // Total tracks

  if (MChargedHadronRAA->highPurity->at(j) == false) return false;
  hNTrkPassCuts->Fill(2); // High purity

  double RelativeUncertainty = MChargedHadronRAA->trkPtError->at(j) / MChargedHadronRAA->trkPt->at(j);
  if (MChargedHadronRAA->trkPt->at(j) > 10 && RelativeUncertainty > 0.1) return false;
  hNTrkPassCuts->Fill(3); // Relative uncertainty < 10%

  if (fabs(MChargedHadronRAA->trkDxyAssociatedVtx->at(j)) / MChargedHadronRAA->trkDxyErrAssociatedVtx->at(j) > 3) return false;
  hNTrkPassCuts->Fill(4); // Dxy < 3 sigma

  if(fabs(MChargedHadronRAA->trkDzAssociatedVtx->at(j)) / MChargedHadronRAA->trkDzErrAssociatedVtx->at(j) > 3) return false;
  hNTrkPassCuts->Fill(5); // Dz < 3 sigma

  if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1) return false;
  hNTrkPassCuts->Fill(6); // Eta < 1, changed from 2.4

  if (MChargedHadronRAA->trkPt->at(j) > 500) return false;
  hNTrkPassCuts->Fill(7); // pT < 500 GeV/c

  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt;
  TH2D *hTrkPtEta;
  TH1D *hNEvtPassCuts, *hNTrkPassCuts;
  ChargedHadronRAATreeMessenger *MChargedHadronRAA;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MChargedHadronRAA(new ChargedHadronRAATreeMessenger(*inf, string("Tree"))),
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

    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", 100, 0, 20);
    hTrkPt->Sumw2();
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", 40, 0, 20, 50, -4.0, 4.0);
    hTrkPtEta->Sumw2();

    hNEvtPassCuts = new TH1D("hNEvtPassCuts", "Number of events passing cuts", 6, 0.5, 6.5);
    hNEvtPassCuts->GetXaxis()->SetBinLabel(1, "Total Events");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(2, "+ CC");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(3, "+ PV");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(4, "+ !isFakeVtx");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(5, "+ abs(VZ)<15");
    hNEvtPassCuts->GetXaxis()->SetBinLabel(6, "+ nTrk>=0");
    hNEvtPassCuts->Sumw2();

    hNTrkPassCuts = new TH1D("hNTrkPassCuts", "Number of tracks passing cuts", 7, 0.5, 7.5);
    hNTrkPassCuts->GetXaxis()->SetBinLabel(1, "Total Tracks");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(2, "+ High Purity");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(3, "+ pT > 10 && Rel pT Error < 10%");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(4, "+ Dxy < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(5, "+ Dz < 3 sigma");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(6, "+ Eta < 1");
    hNTrkPassCuts->GetXaxis()->SetBinLabel(7, "+ pT < 500 GeV/c");
    hNTrkPassCuts->Sumw2();

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    int eventsRejected = 0;
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // event-level
      // event selection criteria
      if (!eventSelection(MChargedHadronRAA, hNEvtPassCuts)) {
        eventsRejected++;
        continue;
      }

      // track-level
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {

        // apply track efficiency correction
        double trkWeight = 1.0;
        if (par.UseTrackWeight) {
          trkWeight *= MChargedHadronRAA->trackWeight->at(j);
        }

        // track selection criteria
        if (!trackSelection(MChargedHadronRAA, j, hNTrkPassCuts)) continue;

        // fill histograms
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j), trkWeight);
        
      } // end of track loop
    } // end of event loop

    cout << "Total events: " << nEntry << endl;
    cout << "Events rejected: " << eventsRejected << endl;
    cout << "Events passing cuts: " << nEntry - eventsRejected << endl;

  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hTrkPt);
    smartWrite(hTrkPtEta);
    smartWrite(hNEvtPassCuts);
    smartWrite(hNTrkPassCuts);
  }

private:
  void deleteHistograms() {
    delete hTrkPt;
    delete hTrkPtEta;
    delete hNEvtPassCuts;
    delete hNTrkPassCuts;
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  // if (printHelpMessage(argc, argv))
  //   return 0;
  CommandLine CL(argc, argv);
  float MinTrackPt = CL.GetDouble("MinTrackPt", 1.0); // Minimum track pT selection
  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  float scaleFactor = CL.GetDouble("ScaleFactor", 1.0);
  int TriggerChoice = CL.GetInt("TriggerChoice", 0);

  Parameters par(MinTrackPt, TriggerChoice, IsData, scaleFactor);
  par.input = CL.Get("Input", "input.root");    // Input file
  par.output = CL.Get("Output", "output.root"); // Output file
  par.UseTrackWeight = CL.GetBool("UseTrackWeight", false); // Path to track efficiency corrections
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
