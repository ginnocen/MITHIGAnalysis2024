#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TTree.h>
#include <algorithm> // for std::min
#include <iostream>
#include <vector>
void loop() {

  gStyle->SetOptStat(0);
  // Open the ROOT file
  TFile *f = TFile::Open("20250703_pO_PhysicsIonPhysics0_393952_full.root");
  // TFile *f = TFile::Open("20250703_pO_PhysicsIonPhysics0_393952_full.root");
  if (!f || f->IsZombie()) {
    std::cerr << "Could not open file!\n";
    return;
  }

  TTree *t = nullptr;
  f->GetObject("Tree", t); // ← update if your tree is named differently
  if (!t) {
    std::cerr << "Tree not found!\n";
    f->Close();
    return;
  }

  // x axis = number of tracks, y axis = number of events
  TH1F *hNtrNoCond = new TH1F("hNtrNoCond", "; n. HP tracks |#eta|<2.4, p_{T}>0.3 GeV; Entries", 1000, -0.5, 999.5);
  TH1F *hNtrPPS = new TH1F("hNtrPPS", "; n. HP tracks |#eta|<2.4, p_{T}>0.3 GeV; Entries", 1000, -0.5, 999.5);
  TH2F *hZDCpvsZDCmNoCond = new TH2F("hZDCpvsZDCmNoCond", "; ZDC+; ZDC-", 100, 0, 8000, 100, 0, 8000);
  TH2F *hZDCpvsZDCmPPS = new TH2F("hZDCpvsZDCmPPS", "; ZDC+; ZDC-", 100, 0, 8000, 100, 0, 8000);
  TH1F *hZDCmNoCond = new TH1F("hZDCmNoCond", "; ZDC-; Entries", 100, 0, 8000);
  TH1F *hZDCmPPS = new TH1F("hZDCmPPS", "; ZDC-; Entries", 100, 0, 8000);
  TH1F *hZDCpNoCond = new TH1F("hZDCpNoCond", "; ZDC+; Entries", 100, 0, 8000);
  TH1F *hZDCpPPS = new TH1F("hZDCpPPS", "; ZDC+; Entries", 100, 0, 8000);
  TH1F *hFSC2bottomM_adcNoCond = new TH1F("hFSC2bottomM_adcNoCond", "; FSC2bottomM_adcNoCond; Entries", 100, 0, 256);
  TH1F *hFSC2bottomM_adcPPS = new TH1F("hFSC2bottomM_adcPPS", "; FSC2bottomM_adcPPS; Entries", 100, 0, 256);
  TH1F *hFSC2bottomM_chargefCNoCond =
      new TH1F("hFSC2bottomM_chargefCNoCond", "; FSC2bottomM_chargefCNoCond; Entries", 10000, 0, 400000);
  TH1F *hFSC2bottomM_chargefCPPS =
      new TH1F("hFSC2bottomM_chargefCPPS", "; FSC2bottomM_chargefCPPS; Entries", 10000, 0, 400000);
  bool passBaselineEventSelection = false;
  float VZ;
  float VZError;
  float ZDCsumMinus, ZDCsumPlus;
  float HFEMaxMinus, HFEMaxPlus;
  bool HLT_OxyZeroBias_v1 = false;

  std::vector<float> *trkPt = nullptr;
  std::vector<float> *trkEta = nullptr;
  std::vector<float> *trkPhi = nullptr;
  std::vector<bool> *highPurity = nullptr;

  std::vector<int> *PPSStation0M_x = nullptr;
  std::vector<float> *PPSStation0M_y = nullptr;
  std::vector<float> *PPSStation2M_x = nullptr;
  std::vector<float> *PPSStation2M_y = nullptr;
  std::vector<int> *FSC2topM_adc = nullptr;
  std::vector<int> *FSC2bottomM_adc = nullptr;
  std::vector<int> *FSC3bottomleftM_adc = nullptr;
  std::vector<int> *FSC3bottomrightM_adc = nullptr;
  std::vector<int> *FSC3topleftM_adc = nullptr;
  std::vector<int> *FSC3toprightM_adc = nullptr;
  std::vector<float> *FSC2topM_chargefC = nullptr;
  std::vector<float> *FSC2bottomM_chargefC = nullptr;
  std::vector<float> *FSC3bottomleftM_chargefC = nullptr;
  std::vector<float> *FSC3bottomrightM_chargefC = nullptr;
  std::vector<float> *FSC3topleftM_chargefC = nullptr;
  std::vector<float> *FSC3toprightM_chargefC = nullptr;

  t->SetBranchAddress("VZ", &VZ);
  t->SetBranchAddress("VZError", &VZError);
  t->SetBranchAddress("ZDCsumMinus", &ZDCsumMinus);
  t->SetBranchAddress("ZDCsumPlus", &ZDCsumPlus);
  t->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
  t->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);

  t->SetBranchAddress("HLT_OxyZeroBias_v1", &HLT_OxyZeroBias_v1);

  t->SetBranchAddress("passBaselineEventSelection", &passBaselineEventSelection);
  t->SetBranchAddress("trkPt", &trkPt);
  t->SetBranchAddress("trkEta", &trkEta);
  t->SetBranchAddress("trkPhi", &trkPhi);
  t->SetBranchAddress("highPurity", &highPurity);

  t->SetBranchAddress("PPSStation0M_x", &PPSStation0M_x);
  t->SetBranchAddress("PPSStation0M_y", &PPSStation0M_y);
  t->SetBranchAddress("PPSStation2M_x", &PPSStation2M_x);
  t->SetBranchAddress("PPSStation2M_y", &PPSStation2M_y);
  t->SetBranchAddress("FSC2topM_adc", &FSC2topM_adc);
  t->SetBranchAddress("FSC2bottomM_adc", &FSC2bottomM_adc);
  t->SetBranchAddress("FSC3bottomleftM_adc", &FSC3bottomleftM_adc);
  t->SetBranchAddress("FSC3bottomrightM_adc", &FSC3bottomrightM_adc);
  t->SetBranchAddress("FSC3topleftM_adc", &FSC3topleftM_adc);
  t->SetBranchAddress("FSC3toprightM_adc", &FSC3toprightM_adc);
  t->SetBranchAddress("FSC2topM_chargefC", &FSC2topM_chargefC);
  t->SetBranchAddress("FSC2bottomM_chargefC", &FSC2bottomM_chargefC);
  t->SetBranchAddress("FSC3bottomleftM_chargefC", &FSC3bottomleftM_chargefC);
  t->SetBranchAddress("FSC3bottomrightM_chargefC", &FSC3bottomrightM_chargefC);
  t->SetBranchAddress("FSC3topleftM_chargefC", &FSC3topleftM_chargefC);
  t->SetBranchAddress("FSC3toprightM_chargefC", &FSC3toprightM_chargefC);

  const Long64_t nEntries = t->GetEntries();
  const Long64_t nMax = std::min<Long64_t>(nEntries, 1000000000000); // cap to 1000 as in your code
  int multHighPurity2p4Pt0p3 = 0;

  for (Long64_t i = 0; i < nMax; ++i) {
    t->GetEntry(i);
    multHighPurity2p4Pt0p3 = 0;
    if (!passBaselineEventSelection) {
      continue;
    }
    // if (HFEMaxPlus > 8.0 || HFEMaxMinus > 8.0) {
    //   continue;
    // }

    if (!HLT_OxyZeroBias_v1) {
      continue;
    } // skip events not passing the trigger

    if (fabs(VZ) > 15.0)
      continue;

    // std::cout << "Event " << i << ": passBaselineEventSelection = " << passBaselineEventSelection << "\n";

    for (size_t j = 0; j < trkPt->size(); ++j) {
      if (highPurity->at(j)) {
        if (trkPt->at(j) > 0.3 && fabs(trkEta->at(j)) < 2.4) {
          multHighPurity2p4Pt0p3++;
        }
      }

      // std::cout << "  Track " << j << ": Pt = " << trkPt->at(j) << ", Eta = " << trkEta->at(j) << ", Phi = " <<
      // trkPhi->at(j) << "\n";
    }
    int size_PPSStation0M_x = PPSStation0M_x->size();
    int size_PPSStation0M_y = PPSStation0M_y->size();
    int size_PPSStation2M_x = PPSStation2M_x->size();
    int size_PPSStation2M_y = PPSStation2M_y->size();
    if (size_PPSStation0M_x > 0 && size_PPSStation0M_y > 0 && size_PPSStation2M_x > 0 && size_PPSStation2M_y > 0) {
      hNtrPPS->Fill(multHighPurity2p4Pt0p3);
      hZDCpvsZDCmPPS->Fill(ZDCsumPlus, ZDCsumMinus);
      hZDCmPPS->Fill(ZDCsumMinus);
      hZDCpPPS->Fill(ZDCsumPlus);
      hFSC2bottomM_adcPPS->Fill(FSC2bottomM_adc->at(2));
      hFSC2bottomM_chargefCPPS->Fill(FSC2bottomM_chargefC->at(2));
    }
    hNtrNoCond->Fill(multHighPurity2p4Pt0p3);
    hZDCpvsZDCmNoCond->Fill(ZDCsumPlus, ZDCsumMinus);
    hZDCmNoCond->Fill(ZDCsumMinus);
    hZDCpNoCond->Fill(ZDCsumPlus);
    hFSC2bottomM_adcNoCond->Fill(FSC2bottomM_adc->at(2));
    hFSC2bottomM_chargefCNoCond->Fill(FSC2bottomM_chargefC->at(2));
  }

  TFile *outFile = TFile::Open("output.root", "RECREATE");
  hNtrNoCond->Write();
  hNtrPPS->Write();
  hZDCpvsZDCmNoCond->Write();
  hZDCpvsZDCmPPS->Write();
  hZDCmNoCond->Write();
  hZDCmPPS->Write();
  hZDCpNoCond->Write();
  hZDCpPPS->Write();
  hFSC2bottomM_adcNoCond->Write();
  hFSC2bottomM_adcPPS->Write();
  hFSC2bottomM_chargefCNoCond->Write();
  hFSC2bottomM_chargefCPPS->Write();
  outFile->Close();
  f->Close();
}

int main() {
  loop();
  return 0;
}
