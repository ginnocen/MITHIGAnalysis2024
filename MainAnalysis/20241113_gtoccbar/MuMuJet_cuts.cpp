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

  //histograms
  TH1D *hMuMuMass;
  TH1D *hMuMupT;
  TH1D *hMupT1;
  TH1D *hMupT2;
  TH2D *hMuMupT1pT2;
  TH1D *hJetpT;
  TH1D *hAsymmetry;
  TH1D *hdR;
  TH1D *hMuJetFraction;
  TH2D *hdRpT;
  TH2D *hdRjetpT;



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

  void analyze(Parameters &par,
  float min_dR, 
  float max_dR,
  float min_di_pT,
  float max_di_pT,
  float min_asymmetry,
  float max_asymmetry,
  float max_drjetmu
  ) {
    outf->cd();

    //histograms
    hMuMuMass = new TH1D(Form("hMuMuMass%s", title.c_str()), "", 500, 0, 50);
    hMuMupT = new TH1D(Form("hMuMuPt%s", title.c_str()), "", 50, 0, 200);
    hMupT1 = new TH1D(Form("hMuPt1%s", title.c_str()), "", 50, 0, 200);
    hMupT2 = new TH1D(Form("hMuPt2%s", title.c_str()), "", 50, 0, 200);
    hMuMupT1pT2 = new TH2D("hMupT1pT2","hMupT1pT2",200,0,500,200,0,500);
    hJetpT = new TH1D(Form("hJetpT%s", title.c_str()), "", 200, 0, 500);
    hAsymmetry = new TH1D(Form("hAsymmetry%s", title.c_str()), "", 20, 0, 1);
    hdR = new TH1D(Form("hdR%s", title.c_str()), "", 150, 0, 3.1416);
    hMuJetFraction = new TH1D(Form("hMuJetFraction%s", title.c_str()), "", 20, 0, 1);
    hdRpT = new TH2D("hdRpT","hdRpT",150,0,3.1416,80,0,200);
    hdRjetpT = new TH2D("hdRjetpT","hdRjetpT",150,0,3.1416,200,0,500);

    unsigned long nentries = MMuMuJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);

    float MuMu_dR = 0;
    float MuMu_di_pT = 0;
    float MuMu_asymmetry = 0;
   

    for (int i = 0; i < MMuMuJet->GetEntries(); i++) {
      MMuMuJet->GetEntry(i);
      if (i % 1000 == 0) {
	      Bar.Update(i);
	      Bar.Print();
      }
      if (!eventSelection(MMuMuJet, par))
	continue;
      for (int j = 0; j < MMuMuJet->mumuMass->size(); j++){


        // muons in the same jet
        if((MMuMuJet->DRJetmu1->at(j) > max_drjetmu)||(MMuMuJet->DRJetmu2->at(j) > max_drjetmu)){
            continue;
        };
        
        // dR condition
        MuMu_dR = MMuMuJet->muDR->at(j); 
        if((MuMu_dR > max_dR)||(MuMu_dR < min_dR)){ 
            continue;

        } 

        // pT condition 
        MuMu_di_pT = MMuMuJet->mumuPt->at(j);   
        if((MuMu_di_pT > max_di_pT)||(MuMu_di_pT < min_di_pT)){
            continue;
        } 

        // asymmetry condition
        MuMu_asymmetry = abs(MMuMuJet->muPt1->at(j) - MMuMuJet->muPt2->at(j))/(MMuMuJet->muPt1->at(j) + MMuMuJet->muPt2->at(j));      
        if((MuMu_asymmetry > max_asymmetry)||(MuMu_asymmetry < min_asymmetry)){
            continue;
        }
        
      

        hMuMuMass->Fill(MMuMuJet->mumuMass->at(j));
        hMuMupT->Fill(MMuMuJet->mumuPt->at(j));
        hMupT1->Fill(MMuMuJet->muPt1->at(j));
        hMupT2->Fill(MMuMuJet->muPt2->at(j));
        hMuMupT1pT2->Fill(MMuMuJet->muPt1->at(j),MMuMuJet->muPt2->at(j));
        hJetpT->Fill(MMuMuJet->JetPT->at(j));
        hAsymmetry->Fill(MuMu_asymmetry);
        hdR->Fill(MuMu_dR);
        hMuJetFraction->Fill(1.0*MMuMuJet->mumuPt->at(j)/MMuMuJet->JetPT->at(j));
        hdRpT->Fill(MuMu_dR,MMuMuJet->mumuPt->at(j));
        hdRjetpT->Fill(MuMu_dR,MMuMuJet->JetPT->at(j));
        
        
        nt->Fill(MMuMuJet->mumuMass->at(j));
      }
    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hMuMuMass);
    smartWrite(hMuMupT);
    smartWrite(hMupT1);
    smartWrite(hMupT2);
    smartWrite(hMuMupT1pT2);
    smartWrite(hJetpT);
    smartWrite(hAsymmetry);
    smartWrite(hdR);
    smartWrite(hMuJetFraction);
    smartWrite(hdRpT);
    smartWrite(hdRjetpT);
    smartWrite(nt);
  }

private:
  void deleteHistograms() { 
    delete hMuMuMass; 
    delete hMuMupT;
    delete hMupT1;
    delete hMupT2;
    delete hMuMupT1pT2;
    delete hJetpT;
    delete hAsymmetry;
    delete hdR;
    delete hMuJetFraction;
    delete hdRpT;
    delete hdRjetpT; }
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

  float min_dR = CL.GetDouble("min_dR", 0);
  float max_dR = CL.GetDouble("max_dR", 3.1416);
  float min_di_pT = CL.GetDouble("min_di_pT", 0);
  float max_di_pT = CL.GetDouble("max_di_pT", 500);
  float min_asymmetry = CL.GetDouble("min_asymmetry", 0);
  float max_asymmetry = CL.GetDouble("max_asymmetry", 1);
  float max_drjetmu = CL.GetDouble("max_drjetmu", 3.1416);
     
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "Data");
  analyzer.analyze(par, min_dR, max_dR, min_di_pT, max_di_pT, min_asymmetry, max_asymmetry, max_drjetmu);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
