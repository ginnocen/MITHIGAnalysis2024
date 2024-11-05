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

//======= trackSelection =====================================//
// Check if the track pass selection criteria
//============================================================//
bool dzeroSelection(DzeroUPCTreeMessenger *b, Parameters par, int j) {
  if ((*b->Dalpha)[j]>par.MaxDalpha) return false;
  return true;
}

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(DzeroUPCTreeMessenger *b, const Parameters &par) {
  return 1;
}

//============================================================//
// Z hadron dphi calculation
//============================================================//
bool getCorrectedYields(DzeroUPCTreeMessenger *MDzeroUPC, TH1D *hDmass,
                        const Parameters &par, TNtuple *nt = 0) {
  hDmass->Sumw2();
  par.printParameters();
  unsigned long nEntry = MDzeroUPC->GetEntries() * par.scaleFactor;
  unsigned long iStart = nEntry * (par.nChunk - 1) / par.nThread;
  unsigned long iEnd = nEntry * par.nChunk / par.nThread;

  ProgressBar Bar(cout, iEnd - iStart);
  Bar.SetStyle(1);
  unsigned long mix_i = iStart;
  unsigned long mixstart_i = mix_i;
  iStart = 0; iEnd = MDzeroUPC->GetEntries();
  for (unsigned long i = iStart; i < iEnd; i++) {
    MDzeroUPC->GetEntry(i);
    if (i%1000==0) {
      Bar.Update(i - iStart);
      Bar.Print();
    }
    // Check if the event passes the selection criteria
    if (eventSelection(MDzeroUPC, par)) {
      for (unsigned long j = 0; j < MDzeroUPC->Dalpha->size(); j++) {
         if (MDzeroUPC->Dpt->at(j) < par.MinDzeroPT) continue;
         if (MDzeroUPC->Dpt->at(j) > par.MaxDzeroPT) continue;
         if (MDzeroUPC->Dy->at(j) < par.MinDzeroY) continue;
         if (MDzeroUPC->Dy->at(j) > par.MaxDzeroY) continue;
         if (MDzeroUPC->Dalpha->at(j) < par.MaxDalpha) continue;
	 if (!MDzeroUPC->DpassCut) continue;
         hDmass->Fill((*MDzeroUPC->Dmass)[j]);
	 nt->Fill((*MDzeroUPC->Dmass)[j]);
      }
    }
  }
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hDmass;
  DzeroUPCTreeMessenger *MDzeroUPC;
  TNtuple *nt;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename,
               const char *mytitle = "Data")
      : inf(new TFile(filename)),
        MDzeroUPC(new DzeroUPCTreeMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt", "D0 mass tree","Dmass");
  }

  ~DataAnalyzer() {
    deleteHistograms();
    delete nt;
    inf->Close();
    outf->Close();
    delete MDzeroUPC;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hDmass = new TH1D(Form("hDmass%s", title.c_str()), "", 60, 1.7, 2.0);
    getCorrectedYields(MDzeroUPC, hDmass, par, nt);
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hDmass);
    smartWrite(nt);
  }

private:
  void deleteHistograms() { delete hDmass; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  std::cout << "Starting DzeroUPC analysis" << std::endl;
  CommandLine CL(argc, argv);
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 1); // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 2); // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", -2); // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", +2); // Maximum Dzero rapidity threshold for Dzero selection.
  float MaxDalpha = CL.GetDouble("MaxDalpha", 0.3); // Minimum Dalpha threshold for Dzero selection.

   Parameters par(MinDzeroPT, MaxDzeroPT, MinDzeroY, MaxDzeroY, MaxDalpha);
   par.input         = CL.Get      ("Input",   "mergedSample/HISingleMuon-v5.root");            // Input file
   par.output        = CL.Get      ("Output",  "output.root");                             	// Output file
   par.nThread       = CL.GetInt   ("nThread", 1);         // The number of threads to be used for parallel processing.
   par.nChunk        = CL.GetInt   ("nChunk", 1);          // Specifies which chunk (segment) of the data to process, used in parallel processing.
   if (checkError(par)) return -1;
   std::cout << "Parameters are set" << std::endl; 
   // Analyze Data
   DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "Data");
   analyzer.analyze(par);
   analyzer.writeHistograms(analyzer.outf);
   cout <<endl<<endl<<"Good good"<<endl;
   saveParametersToHistograms(par, analyzer.outf);   
   cout << "done!" << analyzer.outf->GetName() << endl;
}
