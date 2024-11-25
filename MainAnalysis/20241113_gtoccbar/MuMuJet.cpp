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
  TH1D *hmumuMass;
  TH1D *hjetPT;
  TH1D *hjetEta;
  TH1D *hmuPt1;
  TH1D *hmuPt2;
  TH2D *hmuPt2muPt1;
  TH1D *hDRJetmu1;
  TH1D *hDRJetmu2;
  TH1D *hmuDR;
  MuMuJetMessenger *MMuMuJet;
  TNtuple *nt;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename,
               const char *mytitle = "Data")
      : inf(new TFile(filename)),
        MMuMuJet(new MuMuJetMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt", "#mu^{+}#mu^{-} jet", "mumuMass");
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
    hmumuMass = new TH1D(Form("hmumuMass%s", title.c_str()), "", 500, 0, 50);
    hjetPT = new TH1D(Form("hjetPT%s", title.c_str()), "", 500, 0, 500);
    hjetEta = new TH1D(Form("hjetEta%s", title.c_str()), "", 50, -5, 5);
    hmuPt1 = new TH1D(Form("hmuPt1%s", title.c_str()), "", 500, 0, 500);
    hmuPt2 = new TH1D(Form("hmuPt2%s", title.c_str()), "", 500, 0, 500);
    hmuPt2muPt1 = new TH2D(Form("hmuPt2muPt1%s", title.c_str()), "", 50, 0, 50, 50, 0, 50);
    hDRJetmu1 = new TH1D(Form("hDRJetmu1%s", title.c_str()), "", 50, 0, 0.5);
    hDRJetmu2 = new TH1D(Form("hDRJetmu2%s", title.c_str()), "", 50, 0, 0.5);
    hmuDR = new TH1D(Form("hmuDR%s", title.c_str()), "", 50, 0, 5);
    unsigned long nentries = MMuMuJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    std::cout<<"par.MinJetPT = "<<par.MinJetPT<< ", par.MaxJetPT = "<<par.MaxJetPT<<std::endl;
    Bar.SetStyle(1);
    for (int i = 0; i < MMuMuJet->GetEntries(); i++) {
      MMuMuJet->GetEntry(i);
      if (i % 1000 == 0) {
	Bar.Update(i);
	Bar.Print();
      }
      if (!eventSelection(MMuMuJet, par))
	continue;
      for (int j = 0; j < MMuMuJet->mumuMass->size(); j++){
        if (MMuMuJet->IsMuMuTagged->at(j) == false) continue;
        if (MMuMuJet->JetPT->at(j) < par.MinJetPT || MMuMuJet->JetPT->at(j) > par.MaxJetPT)
	  continue;
        hjetPT->Fill(MMuMuJet->JetPT->at(j));
        hjetEta->Fill(MMuMuJet->JetEta->at(j));
        hmumuMass->Fill(MMuMuJet->mumuMass->at(j));
        hmuPt1->Fill(MMuMuJet->muPt1->at(j));
        hmuPt2->Fill(MMuMuJet->muPt2->at(j));
        hmuPt2muPt1->Fill(MMuMuJet->muPt1->at(j), MMuMuJet->muPt2->at(j));
        hDRJetmu1->Fill(MMuMuJet->DRJetmu1->at(j));
        hDRJetmu2->Fill(MMuMuJet->DRJetmu2->at(j));
        hmuDR->Fill(MMuMuJet->muDR->at(j));
        nt->Fill(MMuMuJet->mumuMass->at(j));
      }
    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hmumuMass);
    smartWrite(hjetPT);
    smartWrite(hjetEta);
    smartWrite(hmuPt1);
    smartWrite(hmuPt2);
    smartWrite(hmuPt2muPt1);
    smartWrite(hDRJetmu1);
    smartWrite(hDRJetmu2);
    smartWrite(hmuDR);
    smartWrite(nt);
  }

private:
  void deleteHistograms() { delete hmumuMass; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPT", 80); // Minimum jet pT
  float MaxJetPT = CL.GetDouble("MaxJetPT", 100); // Maximum jet pT
  bool IsData = CL.GetBool("IsData", 1); // Data or MC
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, MaxJetPT, TriggerChoice, IsData, scaleFactor);
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
