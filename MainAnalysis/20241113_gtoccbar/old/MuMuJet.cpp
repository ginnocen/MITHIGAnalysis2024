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
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions

//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters &par) {
  return false;
}

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(MuMuJetMessenger *b, const Parameters &par) {
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hMuMuMass;
  MuMuJetMessenger *MMuMuJet;
  TNtuple *nt;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename,
               const char *mytitle = "Data")
      : inf(new TFile(filename)),
        MMuMuJet(new MuMuJetMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt", "#mu^{+}#mu^{-} jet", "MuMuMass");
  }

  ~DataAnalyzer() {
    deleteHistograms();
    delete nt;
    inf->Close();
    outf->Close();
    delete MMuMuJet;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hMuMuMass = new TH1D(Form("hMuMuMass%s", title.c_str()), "", 500, 0, 50);
    unsigned long nentries = MMuMuJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);
    for (int i = 0; i < MMuMuJet->GetEntries(); i++) {
      MMuMuJet->GetEntry(i);
      if (i % 1000 == 0) {
	Bar.Update(i);
	Bar.Print();
      }
      if (!eventSelection(MMuMuJet, par))
	continue;
      for (int j = 0; j < MMuMuJet->MuMuMass->size(); j++){
        hMuMuMass->Fill(MMuMuJet->MuMuMass->at(j));
        nt->Fill(MMuMuJet->MuMuMass->at(j));
      }
    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hMuMuMass);
    smartWrite(nt);
  }

private:
  void deleteHistograms() { delete hMuMuMass; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPt", 80); // Minimum jet pT
  bool IsData = CL.GetBool("IsData", 1); // Data or MC
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, TriggerChoice, IsData, scaleFactor);
  par.input         = CL.Get      ("Input",   "mergedSample.root");            // Input file
  par.output        = CL.Get      ("Output",  "output.root");                             	// Output file
  par.nThread       = CL.GetInt   ("nThread", 1);         // The number of threads to be used for parallel processing.
  par.nChunk        = CL.GetInt   ("nChunk", 1);          // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par)) return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "Data");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
