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

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(ChargedHadronRAATreeMessenger *ch, const Parameters &par) {

  // Z Vertex cut 
  if(fabs(ch->VZ) >= par.VzMax){
    return false;
  }

  // NVtx cut
  if(ch->nVtx < par.NVtxMin) {
    return false;
  }

  // Cluster Compatibility Filter
  if(ch->ClusterCompatibilityFilter < par.CCFilter) {
    return false;
  }

  // PV selection
  if(ch->PVFilter < par.PVFilter) {
    return false;
  }

  // Hijing Npart > 1 
  if(par.IsHijing && ch->Npart <= 1) {
    return false;
  }
  
  // HFE energy cuts
  float hiHF = max(ch->HFEMaxPlus, ch->HFEMaxMinus);
  float loHF = min(ch->HFEMaxPlus, ch->HFEMaxMinus);
  if(hiHF < par.HFE_min1 || loHF < par.HFE_min2) {
    return false;
  }

  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hNcoll; 
  TH1D *hNpart;
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
    hNcoll = new TH1D(Form("hNColl%s", title.c_str()), "", 61, -0.5, 60.5);
    hNcoll->Sumw2();
    hNpart = new TH1D(Form("hNpart%s", title.c_str()), "", 100, 0, 100);
    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", 100, 0, 10);
    hTrkPt->Sumw2();
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", 40, 0, 20, 50, -4.0, 4.0);
    hTrkPtEta->Sumw2();
    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }
      MChargedHadronRAA->GetEntry(i);
      if(!eventSelection(MChargedHadronRAA, par)) {continue;}
      hNcoll->Fill(MChargedHadronRAA->Ncoll);
      hNpart->Fill(MChargedHadronRAA->Npart);
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j));
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j));
      }
    } // end of event loop
  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hNcoll);
    smartWrite(hNpart);
    smartWrite(hTrkPt);
    smartWrite(hTrkPtEta);
  }

private:
  void deleteHistograms() {
    delete hNcoll;
    delete hNpart;
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
  
  // EVENT SELECTION Parameters
  float VzMax = CL.GetDouble("Vzmax", 15.0); // Maximum Z vertex position. Set to very high to disable the cut
  int NVtxMin = CL.GetInt("NVtxMin", 1); // Minimum number of vertices. Set to 0 to disable the cut
  int CCFilter = CL.GetInt("CCFilter", 1); // Cluster Compatibility Filter. Set to 0 to disable the cut
  int PVFilter = CL.GetInt("PVFilter", 1); // Primary vertex filter. Set to 0 to disable the cut
  int IsHijing = CL.GetInt("IsHijing", 0); // Flags sample as Hijing, and imposes Npart > 1 cut. Set to 0 to disable the cut

  float HFE_min1 = CL.GetDouble("HFE_min1", 4.0); // Minimum energy for HF
  float HFE_min2 = CL.GetDouble("HFE_min2", 4.0); // Minimum subleading energy for the HF's opposite side. 
  // AND CONDITION: HFE_min1 = HFE_min2
  // OR CONDITION:  HFE_min2 = 0.0
  // ASYMMETRIC CONDITION: HFE_min1 != HFE_min2

  Parameters par(MinTrackPt, TriggerChoice, IsData, scaleFactor, VzMax, NVtxMin, CCFilter, PVFilter, IsHijing, HFE_min1, HFE_min2);
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


