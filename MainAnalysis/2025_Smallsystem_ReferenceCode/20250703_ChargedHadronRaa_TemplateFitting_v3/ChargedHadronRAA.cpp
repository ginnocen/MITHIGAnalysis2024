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

  // Cluster Compatibility Filter
  if(ch->ClusterCompatibilityFilter < par.CCFilter) {
    return false;
  }

  // PV selection
  if(ch->PVFilter < par.PVFilter) {
    return false;
  }

  if(ch->leadingPtEta1p0_sel < par.MinTrackPt) {
    return false;
  }
  
  // HFE energy cuts
  
  int ohiHF = max(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
  int oloHF = min(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
  if(ohiHF <= par.HFE_min1 || oloHF <= par.HFE_min2) {
    return false;
  }

  float hiHF = max(ch->HFEMaxPlus, ch->HFEMaxMinus);
  float loHF = min(ch->HFEMaxPlus, ch->HFEMaxMinus);
  if(hiHF <= par.HFE_min1 || loHF <= par.HFE_min2) {
    return false;
  }
  

  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TTree *EvtTree;
  TTree *TrackTree;
  TH1D *hMult;
  TH1D *hHFEsum;
  TH1D *hHFEMaxPlus;
  TH1D *hHFEMaxMinus;
  TH1D *hTrkPt;
  TH1D *hTrkEta;  
  TH1D *hTrkDCA;  
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


    TrackTree = new TTree("TrackTree", "TrackTree");
    EvtTree = new TTree("EvtTree", "EvtTree");

    float mult = 0;
    float HFEsum = 0;
    float HFEMaxPlus = 0;
    float HFEMaxMinus = 0;

    EvtTree->Branch("Mult", &mult, "Mult/F");
    EvtTree->Branch("HFEsum", &HFEsum, "HFEsum/F");
    EvtTree->Branch("HFEMaxPlus", &HFEMaxPlus, "HFEMaxPlus/F");
    EvtTree->Branch("HFEMaxMinus", &HFEMaxMinus, "HFEMaxMinus/F");
  
    float trkpt = 0;
    float trketa = 0;
    float trkdca = 0;

    TrackTree->Branch("trkPt", &trkpt, "trkPt/F");
    TrackTree->Branch("trkEta", &trketa, "trkEta/F");
    TrackTree->Branch("trkDCA", &trkdca, "trkDCA/F");

    hMult = new TH1D("hMult", "Multiplicity Distribution", 100, 0, 500);
    hHFEsum = new TH1D("hHFEsum", "HFE Sum Distribution", 100, 0, 100);
    hHFEMaxPlus = new TH1D("hHFEMaxPlus", "HFE Max Plus Distribution", 100, 0, 100);
    hHFEMaxMinus = new TH1D("hHFEMaxMinus", "HFE Max Minus Distribution", 100, 0, 100);

    hTrkPt = new TH1D("hTrkPt", "Track pT Distribution", 100, 0, 10);
    hTrkEta = new TH1D("hTrkEta", "Track Eta Distribution", 100, -2.5, 2.5);
    hTrkDCA = new TH1D("hTrkDCA", "Track DCA Distribution", 100, -0.5, 0.5);

    hMult->Sumw2();
    hHFEsum->Sumw2();
    hHFEMaxPlus->Sumw2();
    hHFEMaxMinus->Sumw2();
    hTrkPt->Sumw2();
    hTrkEta->Sumw2();
    hTrkDCA->Sumw2(); 

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries();
    //float weight = par.xSection / nEntry; 
    float weight = 1.0;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }
      MChargedHadronRAA->GetEntry(i);
      if(!eventSelection(MChargedHadronRAA, par)) {continue;}
      
      HFEsum = MChargedHadronRAA->hiHF_pf;
      HFEMaxPlus = MChargedHadronRAA->HFEMaxPlus;
      HFEMaxMinus = MChargedHadronRAA->HFEMaxMinus;
      
      hHFEsum->Fill(HFEsum);
      hHFEMaxPlus->Fill(HFEMaxPlus);
      hHFEMaxMinus->Fill(HFEMaxMinus);    
      

      mult = 0;
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {

        if(MChargedHadronRAA->trkPt->at(j) < 0.3) {continue;} // Minimum pT cut
        trketa = MChargedHadronRAA->trkEta->at(j);
        if(fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0) {continue;}
        trkpt = MChargedHadronRAA->trkPt->at(j);
        trkdca = MChargedHadronRAA->trkDxyAssociatedVtx->at(j);
        hTrkPt->Fill(trkpt, weight);
        hTrkEta->Fill(trketa, weight);
        hTrkDCA->Fill(trkdca, weight);
        mult += 1;
          //TrackTree->Fill();
      }
      hMult->Fill(mult, weight);
      //EvtTree->Fill();
      
    } 
  } 

  void writeHistograms(TFile *outf, Parameters &par) {
    outf->cd();
    //smartWrite(EvtTree);
    //smartWrite(TrackTree);
    smartWrite(hMult);
    smartWrite(hHFEsum);
    smartWrite(hHFEMaxPlus);  
    smartWrite(hHFEMaxMinus);
    smartWrite(hTrkPt);
    smartWrite(hTrkEta);
    
  }

private:
  void deleteHistograms() {
    delete EvtTree;
    delete TrackTree;
    delete hMult;
    delete hHFEsum;   
    delete hHFEMaxPlus;
    delete hHFEMaxMinus;
    delete hTrkPt;
    delete hTrkEta;
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {

  CommandLine CL(argc, argv);  
  // EVENT SELECTION Parameters
  //float xSection = CL.GetDouble("Xsection", 1.0); // Cross section in b
  float VzMax = CL.GetDouble("Vzmax", 15.0); // Maximum Z vertex position. Set to very high to disable the cut
  int CCFilter = CL.GetInt("CCFilter", 1); // Cluster Compatibility Filter. Set to 0 to disable the cut
  int PVFilter = CL.GetInt("PVFilter", 1); // Primary vertex filter. Set to 0 to disable the cut
  
  float MinTrackPt = CL.GetDouble("MinTrackPt", 0); // Minimum track pT. Set to 0.3 GeV/c
  float HFE_min1 = CL.GetDouble("HFE_min_1", 0); // Minimum leading energy for HF
  float HFE_min2 = CL.GetDouble("HFE_min_2", 0); // Minimum subleading energy for the HF's opposite side. 
  float ADC_min1 = CL.GetDouble("ADC_min_1", 14); // Minimum leading ADC for online HF
  float ADC_min2 = CL.GetDouble("ADC_min_2", 0); // Minimum subleading ADC for the online HF's opposite side.

  // AND CONDITION: HFE_min1 = HFE_min2
  // OR CONDITION:  HFE_min2 = -1
  // ASYMMETRIC CONDITION: HFE_min1 != HFE_min2

  Parameters par(VzMax, CCFilter, PVFilter, MinTrackPt, HFE_min1, HFE_min2, ADC_min1, ADC_min2);
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


