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

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt;
  TH2D *hTrkPtEta;
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
    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", 100, 0, 10);
    hTrkPt->Sumw2();
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", 40, 0, 20, 50, -4.0, 4.0);
    hTrkPtEta->Sumw2();
    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j));
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j));
      }
    } // end of event loop
  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hTrkPt);
    smartWrite(hTrkPtEta);
  }

private:
  void deleteHistograms() {
    delete hTrkPt;
    delete hTrkPtEta;
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
