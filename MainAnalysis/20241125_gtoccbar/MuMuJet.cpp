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
  TH1D *hTaggedmumuMass;
  TH1D *hInclusivejetPT;
  TH1D *hTaggedjetPT;
  TH1D *hTaggedjetEta;
  TH1D *hTaggedmuPt1;
  TH1D *hTaggedmuPt2;
  TH2D *hTaggedmuPt2muPt1;
  TH1D *hTaggedDRJetmu1;
  TH1D *hTaggedDRJetmu2;
  TH1D *hTaggedmuDR;
  TH1D *hTaggemuPtAsymm;
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
    hInclusivejetPT = new TH1D(Form("hInclusivejetPT%s", title.c_str()), "", 500, 0, 500);
    hTaggedmumuMass = new TH1D(Form("hTaggedmumuMass%s", title.c_str()), "", 500, 0, 50);
    hTaggedjetPT = new TH1D(Form("hTaggedjetPT%s", title.c_str()), "", 500, 0, 500);
    hTaggedjetEta = new TH1D(Form("hTaggedjetEta%s", title.c_str()), "", 50, -5, 5);
    hTaggedmuPt1 = new TH1D(Form("hTaggedmuPt1%s", title.c_str()), "", 500, 0, 500);
    hTaggedmuPt2 = new TH1D(Form("hTaggedmuPt2%s", title.c_str()), "", 500, 0, 500);
    hTaggedmuPt2muPt1 = new TH2D(Form("hTaggedmuPt2muPt1%s", title.c_str()), "", 50, 0, 50, 50, 0, 50);
    hTaggemuPtAsymm = new TH1D(Form("hTaggemuPtAsymm%s", title.c_str()), "", 40, -0.5, 1.5);
    hTaggedDRJetmu1 = new TH1D(Form("hTaggedDRJetmu1%s", title.c_str()), "", 50, 0, 0.5);
    hTaggedDRJetmu2 = new TH1D(Form("hTaggedDRJetmu2%s", title.c_str()), "", 50, 0, 0.5);
    hTaggedmuDR = new TH1D(Form("hTaggedmuDR%s", title.c_str()), "", 50, 0, 5);
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
        if (MMuMuJet->JetPT->at(j) < par.MinJetPT || MMuMuJet->JetPT->at(j) > par.MaxJetPT)
	  continue;
        hInclusivejetPT->Fill(MMuMuJet->JetPT->at(j));
        if (MMuMuJet->IsMuMuTagged->at(j) == false) continue;
        hTaggedjetPT->Fill(MMuMuJet->JetPT->at(j));
        hTaggedjetEta->Fill(MMuMuJet->JetEta->at(j));
        hTaggedmumuMass->Fill(MMuMuJet->mumuMass->at(j));
        hTaggedmuPt1->Fill(MMuMuJet->muPt1->at(j));
        hTaggedmuPt2->Fill(MMuMuJet->muPt2->at(j));
        hTaggedmuPt2muPt1->Fill(MMuMuJet->muPt1->at(j), MMuMuJet->muPt2->at(j));
        hTaggedDRJetmu1->Fill(MMuMuJet->DRJetmu1->at(j));
        hTaggedDRJetmu2->Fill(MMuMuJet->DRJetmu2->at(j));
        hTaggedmuDR->Fill(MMuMuJet->muDR->at(j));
        hTaggemuPtAsymm->Fill((MMuMuJet->muPt1->at(j) - MMuMuJet->muPt2->at(j)) / (MMuMuJet->muPt1->at(j) + MMuMuJet->muPt2->at(j)));
        nt->Fill(MMuMuJet->mumuMass->at(j));
      }
    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hInclusivejetPT);
    smartWrite(hTaggedmumuMass);
    smartWrite(hTaggedjetPT);
    smartWrite(hTaggedjetEta);
    smartWrite(hTaggedmuPt1);
    smartWrite(hTaggedmuPt2);
    smartWrite(hTaggedmuPt2muPt1);
    smartWrite(hTaggedDRJetmu1);
    smartWrite(hTaggedDRJetmu2);
    smartWrite(hTaggedmuDR);
    smartWrite(hTaggemuPtAsymm);
    smartWrite(nt);
  }

private:
  void deleteHistograms() {
    delete hTaggedmumuMass; 
    delete hInclusivejetPT;
    delete hTaggedjetPT;
    delete hTaggedjetEta;
    delete hTaggedmuPt1;
    delete hTaggedmuPt2;
    delete hTaggedmuPt2muPt1;
    delete hTaggedDRJetmu1;
    delete hTaggedDRJetmu2;
    delete hTaggedmuDR;
    delete hTaggemuPtAsymm;
  }
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
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
