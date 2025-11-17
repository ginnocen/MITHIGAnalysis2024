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
#include <vector>

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

//======= FlavorClassifier ===================================//
// Organize MC jets by gen level hadron information
//============================================================//
int FlavorClassifier(int NbHad, int NcHad) {
  if (NbHad == 2)
    return 5; // bbbar
  else if (NbHad == 1)
    return 4; // b
  else if ((NcHad == 2) && (NbHad == 0))
    return 3; // ccbar
  else if ((NcHad == 1) && (NbHad == 0))
    return 2; // c
  else if ((NbHad == 0) && (NcHad == 0))
    return 1; // light
  else 
    return 0; // other
}

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(DimuonJetMessenger *b, const Parameters &par) { return true; }

//======= JetSelection =====================================//
// Check if the event pass JetSelection criteria
//============================================================//
bool jetselection(DimuonJetMessenger *b, const Parameters &par) {

  // DIMUON SELECTION
  if (b->IsMuMuTagged != 1)
    return false;

  // JET PT CUT
  if (b->JetPT < par.MinJetPT || b->JetPT > par.MaxJetPT)
    return false;

  // DCA SELECTION
  TTreeFormula *DCAFormula = new TTreeFormula("dcaSelection", par.DCAString.c_str(), b->Tree);
  if (DCAFormula->EvalInstance() <= 0) {
    return false;
  }
  delete DCAFormula;

  // CHARGE SELECTION
  if (par.ChargeSelection == 1) {
    if (b->muCharge1 != b->muCharge2)
      return false;
  } else if (par.ChargeSelection == -1) {
    if (b->muCharge1 == b->muCharge2)
      return false;
  }

  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  
  // INCLUSIVE HISTOGRAMS
  TH1D *hInclusivejetPT;

  // DIMUON HISTOGRAMS
  TH1D *hJetPT;
  TH1D *hInvMass;
  TH1D *hmuDiDxy1Dxy2Sig;
  TH1D *hmumuPt;
  TH1D *hmuDR;
  TNtuple *nt;
  
  // FLAVOR-SORTED HISTOGRAMS 
  vector<TH1D*> hJetPT_flavors;
  vector<TH1D*> hInvMass_flavors;     
  vector<TH1D*> hmuDiDxy1Dxy2Sig_flavors;
  vector<TH1D*> hmumuPt_flavors;
  vector<TH1D*> hmuDR_flavors;
  vector<TNtuple*> nt_flavors;
  vector<string> flavorNames;

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

    // DECLARE HISTOGRAMS
    hInclusivejetPT = new TH1D(Form("hInclusivejetPT%s", title.c_str()), "", 500, 0, 500);

    hJetPT = new TH1D(Form("hJetPT%s", title.c_str()), "", 500, 0, 500);
    hInvMass = new TH1D(Form("hInvMass%s", title.c_str()), "", 50, 0, 7);
    hmuDiDxy1Dxy2Sig = new TH1D(Form("hmuDiDxy1Dxy2Sig%s", title.c_str()), "", 50, -3, 4);
    hmumuPt = new TH1D(Form("hmumuPt%s", title.c_str()), "", 50, 0, 150);  // FIX: Initialize missing histogram
    hmuDR = new TH1D(Form("hmuDR%s", title.c_str()), "", 50, 0, 0.6);
    nt = new TNtuple(Form("nt%s", title.c_str()), "", "mumuMass:muDiDxy1Dxy2Sig:mumuPt:muDR:JetPT");

    // DECLARE FLAVOR HISTOGRAMS
    if(!par.IsData) {
      flavorNames = {"other", "uds", "c", "cc", "b", "bb"};
      for(int i = 0; i < 6; i++) {
        hJetPT_flavors.push_back(new TH1D(Form("hJetPT_%s", flavorNames[i].c_str()), "", 500, 0, 500));
        hInvMass_flavors.push_back(new TH1D(Form("hInvMass_%s", flavorNames[i].c_str()), "", 50, 0, 7));
        hmuDiDxy1Dxy2Sig_flavors.push_back(new TH1D(Form("hmuDiDxy1Dxy2Sig_%s", flavorNames[i].c_str()), "", 50, -3, 4));
        hmumuPt_flavors.push_back(new TH1D(Form("hmumuPt_%s", flavorNames[i].c_str()), "", 50, 0, 150));
        hmuDR_flavors.push_back(new TH1D(Form("hmuDR_%s", flavorNames[i].c_str()), "", 50, 0, 0.6));
        nt_flavors.push_back(new TNtuple(Form("nt_%s", flavorNames[i].c_str()), "", "mumuMass:muDiDxy1Dxy2Sig:mumuPt:muDR:JetPT"));
      }
    }

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

      // INCLUSIVE HISTOGRAM
      if(MDimuonJet->JetPT > par.MinJetPT && MDimuonJet->JetPT < par.MaxJetPT){
        hInclusivejetPT->Fill(MDimuonJet->JetPT);
      }

      // APPLY JET SELECTION
      if (!jetselection(MDimuonJet, par)) {
        continue;
      }

      // FILL HISTOGRAMS
      hJetPT->Fill(MDimuonJet->JetPT);
      hInvMass->Fill(MDimuonJet->mumuMass);
      hmuDiDxy1Dxy2Sig->Fill(log10(abs(MDimuonJet->muDiDxy1Dxy2 / MDimuonJet->muDiDxy1Dxy2Err)));
      hmumuPt->Fill(MDimuonJet->mumuPt);
      hmuDR->Fill(MDimuonJet->muDR);
      nt->Fill(MDimuonJet->mumuMass, MDimuonJet->muDiDxy1Dxy2 / MDimuonJet->muDiDxy1Dxy2Err, MDimuonJet->mumuPt, MDimuonJet->muDR, MDimuonJet->JetPT);

      // FILL FLAVOR HISTOGRAMS (MC only)
      if (!par.IsData) { 
        int flavorIndex = FlavorClassifier(MDimuonJet->NbHad, MDimuonJet->NcHad);
        hJetPT_flavors[flavorIndex]->Fill(MDimuonJet->JetPT);
        hInvMass_flavors[flavorIndex]->Fill(MDimuonJet->mumuMass);
        hmuDiDxy1Dxy2Sig_flavors[flavorIndex]->Fill(log10(abs(MDimuonJet->muDiDxy1Dxy2 / MDimuonJet->muDiDxy1Dxy2Err)));
        hmumuPt_flavors[flavorIndex]->Fill(MDimuonJet->mumuPt);
        hmuDR_flavors[flavorIndex]->Fill(MDimuonJet->muDR);
        nt_flavors[flavorIndex]->Fill(MDimuonJet->mumuMass, MDimuonJet->muDiDxy1Dxy2 / MDimuonJet->muDiDxy1Dxy2Err, MDimuonJet->mumuPt, MDimuonJet->muDR, MDimuonJet->JetPT);
      }
    }
  }

  void writeHistograms(TFile *outf, Parameters &par) {
    outf->cd();
    smartWrite(hInclusivejetPT);
    smartWrite(hJetPT);
    smartWrite(hInvMass);
    smartWrite(hmuDiDxy1Dxy2Sig);
    smartWrite(hmumuPt);
    smartWrite(hmuDR);
    smartWrite(nt);
    
    // Write flavor-specific histograms
    if(!par.IsData) {
      for(int i = 0; i < 6; i++) {
      smartWrite(hInvMass_flavors[i]);
      smartWrite(hJetPT_flavors[i]);
      smartWrite(hmuDiDxy1Dxy2Sig_flavors[i]);
      smartWrite(hmumuPt_flavors[i]);
      smartWrite(hmuDR_flavors[i]);
      smartWrite(nt_flavors[i]);
      }
    }
  }

private:
  void deleteHistograms(Parameters &par) {
    delete hInclusivejetPT;
    delete hJetPT;
    delete hInvMass;
    delete hmuDiDxy1Dxy2Sig;
    delete hmumuPt;
    delete hmuDR;
    delete nt;

    for(int i = 0; i < 6; i++) {
      delete hInvMass_flavors[i];
      delete hJetPT_flavors[i];
      delete hmuDiDxy1Dxy2Sig_flavors[i];
      delete hmumuPt_flavors[i];
      delete hmuDR_flavors[i];
      delete nt_flavors[i];
    }
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPT", 80);   // Minimum jet pT
  float MaxJetPT = CL.GetDouble("MaxJetPT", 100); // Maximum jet pT
  int ChargeSelection =
      CL.GetInt("ChargeSelection", 0); // Charge selection for dimuon: 0 = no sel, 1 = same sign, -1 = opposite sign
  TString DCAString = CL.Get("DCAString", "");         // DCA selection string
  bool IsData = CL.GetBool("IsData", 1);               // Data or MC
  bool IsPP = CL.GetBool("IsPP", 0);                   // pp or PbPb
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, MaxJetPT, string(DCAString.Data()), ChargeSelection, TriggerChoice, IsData, IsPP, scaleFactor);
  par.input = CL.Get("Input", "mergedfile.root"); // Input file
  par.output = CL.Get("Output", "output.root");   // Output file
  par.nThread = CL.GetInt("nThread", 1);          // The number of threads to be used for parallel processing.
  par.nChunk =
      CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf, par);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
