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
#include "QAMessenger.h"


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

  // Cluster Compatibility Filter
  if(ch->ClusterCompatibilityFilter < par.CCFilter) {
    return false;
  }

  // PV selection
  if(ch->PVFilter < par.PVFilter) {
    return false;
  }
  
  // HFE energy cuts
  if(par.useOnlineHFE){
    int ohiHF = max(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
    int oloHF = min(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
    if(ohiHF <= par.HFE_min1 || oloHF <= par.HFE_min2) {
      return false;
    }
  }
  else{
    float hiHF = max(ch->HFEMaxPlus, ch->HFEMaxMinus);
    float loHF = min(ch->HFEMaxPlus, ch->HFEMaxMinus);
    if(hiHF <= par.HFE_min1 || loHF <= par.HFE_min2) {
      return false;
    }
  }
  

  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hNcoll; 
  TH1D *hNpart;
  TH1D *hTrkMult;
  TH1D *hTrkPt;
  TH1D *hTrkEta;
  TH2D *hTrkPtEta;
  ChargedHadronRAATreeMessenger *MChargedHadronRAA;
  string title;
  QAMessenger *QA;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MChargedHadronRAA(new ChargedHadronRAATreeMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")), QA(new QAMessenger(mytitle)) {
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MChargedHadronRAA;
    delete QA;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hNcoll = new TH1D(Form("hNColl%s", title.c_str()), "", 61, -0.5, 60.5);
    hNpart = new TH1D(Form("hNpart%s", title.c_str()), "", 100, 0, 100);
    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", 100, 0, 10);
    hTrkEta = new TH1D(Form("hTrkEta%s", title.c_str()), "", 50, -3.0, 3.0);
    hTrkPtEta = new TH2D(Form("hTrkPtEta%s", title.c_str()), "", 40, 0, 20, 50, -4.0, 4.0);
    hTrkMult = new TH1D(Form("hTrkMult%s", title.c_str()), "", 501, -0.5, 500.5);

    hNcoll->Sumw2();
    hNpart->Sumw2();
    hTrkPt->Sumw2();
    hTrkEta->Sumw2();
    hTrkPtEta->Sumw2();
    hTrkMult->Sumw2();
  
    int mult = 0;
    par.printParameters();
    if(par.doQA){QA->Initialize(title);}
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    float weight = par.xSection / nEntry; 
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }
      MChargedHadronRAA->GetEntry(i);
      if(!eventSelection(MChargedHadronRAA, par)) {continue;}
      //if(par.IsHijing && MChargedHadronRAA->Npart <= 1) {continue;} // MAY BECOME OBSOLETE. IF SO, WILL REMOVE ISHIJING PARAMETER
      if(par.doQA){QA->AnalyzeEvent(MChargedHadronRAA, weight);}
      hNcoll->Fill(MChargedHadronRAA->Ncoll, weight);
      hNpart->Fill(MChargedHadronRAA->Npart, weight);
      mult = 0;
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        if(fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0) {continue;}
        if(MChargedHadronRAA->trkPt->at(j) > 0.5) {mult +=1;} // Minimum pT cut
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), weight);
        hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), weight);
        hTrkPtEta->Fill(MChargedHadronRAA->trkPt->at(j), MChargedHadronRAA->trkEta->at(j), weight);
      }
      hTrkMult->Fill(mult, weight);
    } // end of event loop
  } // end of analyze

  void writeHistograms(TFile *outf, Parameters &par) {
    outf->cd();
    smartWrite(hNcoll);
    smartWrite(hNpart);
    smartWrite(hTrkMult);
    smartWrite(hTrkPt);
    smartWrite(hTrkEta);
    smartWrite(hTrkPtEta);
    if(par.doQA){QA->WriteHistograms(outf);}
    
  }

private:
  void deleteHistograms() {
    delete hNcoll;
    delete hTrkMult;
    delete hNpart;
    delete hTrkPt;
    delete hTrkEta;
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
  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  float scaleFactor = CL.GetDouble("ScaleFactor", 1.0);
  int TriggerChoice = CL.GetInt("TriggerChoice", 0);
  
  // EVENT SELECTION Parameters
  float xSection = CL.GetDouble("Xsection", 1.0); // Cross section in b
  float VzMax = CL.GetDouble("Vzmax", 15.0); // Maximum Z vertex position. Set to very high to disable the cut
  int CCFilter = CL.GetInt("CCFilter", 1); // Cluster Compatibility Filter. Set to 0 to disable the cut
  int PVFilter = CL.GetInt("PVFilter", 1); // Primary vertex filter. Set to 0 to disable the cut
  int IsHijing = CL.GetInt("IsHijing", 0); // Flags sample as Hijing, and imposes Npart > 1 cut. Set to 0 to disable the cut

  float HFE_min1 = CL.GetDouble("HFE_min_1", 6.0); // Minimum leading energy for HF
  float HFE_min2 = CL.GetDouble("HFE_min_2", 6.0); // Minimum subleading energy for the HF's opposite side. 
  bool useOnlineHFE = CL.GetInt("useOnlineHFE", 0); // Use online HFE cuts. Set to 1 to use online cuts, 0 to use offline cuts

  // AND CONDITION: HFE_min1 = HFE_min2
  // OR CONDITION:  HFE_min2 = 0.0
  // ASYMMETRIC CONDITION: HFE_min1 != HFE_min2
  bool doQA = CL.GetBool("doQA", 1); 

  Parameters par(TriggerChoice, IsData, scaleFactor, xSection, VzMax, CCFilter, PVFilter, IsHijing, HFE_min1, HFE_min2, useOnlineHFE, doQA);
  par.input = CL.Get("Input", "input.root");    // Input file
  par.output = CL.Get("Output", "output.root"); // Output file
  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf, par);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;

  return 0;
}


