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

void loop(bool applyPPSTagCondition = true, bool applyZDCPlusVeto = true, bool applyHFPlusGapVeto = true) {

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
  TH1F *hNtr = new TH1F("hNtr", "; n. HP tracks |#eta|<2.4, p_{T}>0.3 GeV; Entries", 1000, -0.5, 999.5);
  TH1F *hEtaCharged = new TH1F("hEtaCharged", "; #eta; Entries", 100, -5, 5);
  TH1F *hZDCm = new TH1F("hZDCm", "; ZDC-; Entries", 100, 0, 8000);
  TH1F *hZDCp = new TH1F("hZDCp", "; ZDC+; Entries", 100, 0, 8000);
  TH1F *hFSC3bottomleftM_adc = new TH1F("hFSC3bottomleftM_adc", "; FSC3bottomleftM_adc; Entries", 100, 0, 256);
  TH1F *hFSC3botleftM_fC = new TH1F("hFSC3botleftM_fC", "; FSC3bottomleftM_chargefC; Entries", 10000, 0, 400000);
  TH1F *hLeadingTrackPt = new TH1F("hLeadingTrackPt", "; Leading Track p_{T} [GeV]; Entries", 50, 0, 5);

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
  const Long64_t nMax = std::min<Long64_t>(nEntries, 100000000); // cap to 1000 as in your code
  int multHighPurity2p4Pt0p3 = 0;
  std::cout << "Total entries in tree: " << nEntries << "\n";
  for (Long64_t i = 0; i < nMax; ++i) {
    t->GetEntry(i);
    multHighPurity2p4Pt0p3 = 0;
    // printout every 1000 events
    if (i % 100000 == 0) {
      std::cout << "Processing event " << i << "/" << nMax << "\n";
    }

    if (applyHFPlusGapVeto && HFEMaxPlus > 9.0)
      continue;
    if (!HLT_OxyZeroBias_v1) {
      continue;
    } // skip events not passing the trigger

    if (!passBaselineEventSelection) {
      continue;
    }
    if (fabs(VZ) > 15.0)
      continue;

    if (applyZDCPlusVeto && ZDCsumPlus > 1300)
      continue;

    int size_PPSStation0M_x = PPSStation0M_x->size();
    int size_PPSStation0M_y = PPSStation0M_y->size();
    int size_PPSStation2M_x = PPSStation2M_x->size();
    int size_PPSStation2M_y = PPSStation2M_y->size();
    // FIXME: this tagging is very very preliminary,
    // it should be replaced with a proper one that relies on the proton reconstruction, timing, etc.
    bool PPS_tag =
        (size_PPSStation0M_x > 0 && size_PPSStation0M_y > 0 && size_PPSStation2M_x > 0 && size_PPSStation2M_y > 0);

    if (applyPPSTagCondition && !PPS_tag)
      continue;
    float leadingTrackPt = -1.0;

    for (size_t j = 0; j < trkPt->size(); ++j) {
      if (highPurity->at(j)) {
        if (trkPt->at(j) > 0.3 && fabs(trkEta->at(j)) < 2.4) {
          multHighPurity2p4Pt0p3++;
          hEtaCharged->Fill(trkEta->at(j));
          if (trkPt->at(j) > leadingTrackPt) {
            leadingTrackPt = trkPt->at(j);
          }
        } // end of loop pt and eta cut
      } // end of highPurity check
    } // end of track loop
    // if (leadingTrackPt < 2.0) continue;
    hLeadingTrackPt->Fill(leadingTrackPt);
    hNtr->Fill(multHighPurity2p4Pt0p3);
    hZDCm->Fill(ZDCsumMinus);
    hZDCp->Fill(ZDCsumPlus);
    hFSC3bottomleftM_adc->Fill((*FSC3bottomleftM_adc)[2]);
    hFSC3botleftM_fC->Fill((*FSC3bottomleftM_chargefC)[2]);
  } // end of event loop

  TFile *outFile = TFile::Open(Form("output_loop_ppsTag_%d_zdcPlusVeto_%d_HFPlusGapVeto_%d.root", applyPPSTagCondition,
                                    applyZDCPlusVeto, applyHFPlusGapVeto),
                               "RECREATE");
  outFile->cd();
  hLeadingTrackPt->Write();
  hNtr->Write();
  hEtaCharged->Write();
  hZDCm->Write();
  hZDCp->Write();
  hFSC3bottomleftM_adc->Write();
  hFSC3botleftM_fC->Write();
  outFile->Close();
}

int main() {
  loop();
  return 0;
}
