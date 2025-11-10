#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>

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
  TH1D *hInvMass;
  TH1D *hDCA;
  DimuonJetMessenger *MDimuonJet;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "Data")
      : inf(new TFile(filename)), MDimuonJet(new DimuonJetMessenger(*inf, string("Tree"))), title(mytitle),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
  }

  ~DataAnalyzer() {
    // deleteHistograms();
    inf->Close();
    outf->Close();
    delete MDimuonJet;
  }

  void analyze(Parameters &par) {
    outf->cd();

    // HISTOGRAMS
    hInclusivejetPT = new TH1D(Form("hInclusivejetPT_%s", title.c_str()), "", 500, 0, 500);
    hInvMass = new TH1D(Form("hInvMass_%s", title.c_str()), "", 50, 0, 7);
    hDCA = new TH1D(Form("hDCA_%s", title.c_str()), "", 50, -10, 2);
    
    // JET SELECTION
    TString selections = "IsMuMuTagged == 1";
    selections += " && ";
    selections += par.DCAString;
    selections += " && JetPT < ";
    selections += par.MaxJetPT;
    selections += " && JetPT > ";
    selections += par.MinJetPT;
    if(par.ChargeSelection == 1) {
        selections += " && muCharge1 == muCharge2";
    } else if(par.ChargeSelection == -1) {
        selections += " && muCharge1 != muCharge2";
    }
    if(par.NbHad >= 0) {
        selections += " && NbHad == ";
        selections += par.NbHad;
    }
    if(par.NcHad >= 0) {
        selections += " && NcHad == ";
        selections += par.NcHad;
    }
    
    cout << "SELECTION STRING: " << endl;
    cout << selections << endl;
    TTreeFormula* CutFormula = new TTreeFormula("dcaSelection", selections, MDimuonJet->Tree);
    
    // LOOP OVER JETS
    unsigned long nentries = MDimuonJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);
    for (int i = 0; i < MDimuonJet->GetEntries(); i++) {
      MDimuonJet->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // DCA SELECTION using EvalInstance
      if (CutFormula->EvalInstance() <= 0)
        continue; 

      // FILL HISTOGRAMS
      hInclusivejetPT->Fill(MDimuonJet->JetPT);
      hInvMass->Fill(MDimuonJet->mumuMass);
      hDCA->Fill(log10(abs(MDimuonJet->muDiDxy1Dxy2)));

    }
    
    delete CutFormula;
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hInclusivejetPT);
    smartWrite(hInvMass);
    smartWrite(hDCA);
  }

private:
  void deleteHistograms() { delete hInclusivejetPT; delete hInvMass; delete hDCA; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPT", 80);       // Minimum jet pT
  float MaxJetPT = CL.GetDouble("MaxJetPT", 1000);      // Maximum jet pT
  int ChargeSelection = CL.GetInt("ChargeSelection", 0); // Charge selection for dimuon: 0 = no sel, 1 = same sign, -1 = opposite sign
  TString DCAString = CL.Get("DCAString", ""); // DCA selection string
  bool IsData = CL.GetBool("IsData", 1);               // Data or MC
  bool IsPP = CL.GetBool("IsPP", 0);               // pp or PbPb
  int NbHad = CL.GetInt("NbHad", -1); // Number of b hadrons (-1 = no cut)
  int NcHad = CL.GetInt("NcHad", -1); // Number of c hadrons (-1 = no cut)
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, MaxJetPT, string(DCAString.Data()), ChargeSelection, TriggerChoice, IsData, IsPP, NbHad, NcHad, scaleFactor);
  par.input = CL.Get("Input", "mergedfile.root"); // Input file
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
