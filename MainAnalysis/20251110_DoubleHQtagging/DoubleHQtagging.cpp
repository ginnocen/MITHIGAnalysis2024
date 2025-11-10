#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>

#include <iostream>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "DimuonMessenger.h"
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions

//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters &par) { return false; }

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(DimuonJetMessenger *b, const Parameters &par) { return true; }

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hInclusivejetPT;
  DimuonJetMessenger *MDimuonJet;
  // TNtuple *nt;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "Data")
      : inf(new TFile(filename)), MDimuonJet(new DimuonJetMessenger(*inf, string("Tree"))), title(mytitle),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    // nt = new TNtuple("nt", "#mu^{+}#mu^{-} jet", "mumuMass");
  }

  ~DataAnalyzer() {
    // deleteHistograms();
    inf->Close();
    outf->Close();
    delete MDimuonJet;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hInclusivejetPT = new TH1D(Form("hInclusivejetPT%s", title.c_str()), "", 500, 0, 500);
    unsigned long nentries = MDimuonJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);
    for (int i = 0; i < MDimuonJet->GetEntries(); i++) {
      MDimuonJet->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }
      // if (!eventSelection(MDimuonJet, par))
      //	continue;
      if (MDimuonJet->JetPT < par.MinJetPT || MDimuonJet->JetPT > par.MaxJetPT)
        continue;
      hInclusivejetPT->Fill(MDimuonJet->JetPT);
      // nt->Fill(MDimuonJet->mumuMass->at(j));
    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hInclusivejetPT);
    // smartWrite(nt);
  }

private:
  void deleteHistograms() { delete hInclusivejetPT; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPT", 80);       // Minimum jet pT
  float MaxJetPT = CL.GetDouble("MaxJetPT", 100);      // Maximum jet pT
  bool IsData = CL.GetBool("IsData", 1);               // Data or MC
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, MaxJetPT, TriggerChoice, IsData, scaleFactor);
  par.input = CL.Get("Input", "mergedSample.root"); // Input file
  par.output = CL.Get("Output", "output.root");     // Output file
  par.nThread = CL.GetInt("nThread", 1);            // The number of threads to be used for parallel processing.
  par.nChunk =
      CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
