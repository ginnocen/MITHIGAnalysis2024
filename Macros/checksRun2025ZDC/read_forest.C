// read_forest.C
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include "TH1F.h"
void read_forest(const char *in = "HiForest_2025PbPbUPC_crab_PbPbUPC_HIForward1_399540_251119_053056.root",
                 Long64_t maxEvents = 100) {
  bool debug = false;
  TFile *f = TFile::Open(in);
  if (!f || f->IsZombie()) {
    std::cerr << "Can't open file\n";
    return;
  }

  // --- Event tree (hiEvtAnalyzer) ------------------------------------------
  TTree *tEvt = (TTree *)f->Get("hiEvtAnalyzer/HiTree");
  if (!tEvt) {
    std::cerr << "Missing hiEvtAnalyzer/HiTree\n";
    return;
  }

  UInt_t run = 0, lumi = 0;
  ULong64_t evt = 0;
  Float_t vz = 0.f;
  // Add/remove to taste
  tEvt->SetBranchAddress("run", &run);
  tEvt->SetBranchAddress("lumi", &lumi);
  tEvt->SetBranchAddress("evt", &evt);
  tEvt->SetBranchAddress("vz", &vz);

  // --- HLT tree (hltanalysis) [optional: uncomment if needed] --------------
  TTree *tHLT = (TTree *)f->Get("hltanalysis/HltTree");
  if (!tHLT) {
    std::cout << "Warning: missing hltanalysis/HltTree\n";
    return;
  }
  int HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v = 0;
  int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v = 0;
  int HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v = 0;
  int HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v = 0;

  if (tHLT) {
    tHLT->SetBranchAddress("HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v5", &HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v);
    tHLT->SetBranchAddress("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v16",
                           &HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v);
    tHLT->SetBranchAddress("HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v5", &HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v);
    tHLT->SetBranchAddress("HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v16",
                           &HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v);
  } else {
    std::cout << "Warning: missing hltanalysis/HltTree\n";
  }
  TTree *tzdc = (TTree *)f->Get("zdcanalyzer/zdcrechit");
  if (!tzdc) {
    std::cerr << "Missing zdcanalyzer/zdcrechit\n";
    return;
  }

  float sumPlus = -9999;
  float sumMinus = -9999;
  if (tzdc) {
    tzdc->SetBranchAddress("sumPlus", &sumPlus);
    tzdc->SetBranchAddress("sumMinus", &sumMinus);
  }
  // --- Muon tree (muonAnalyzer) [optional: uncomment if needed] ------------
  // TTree* tMu = (TTree*)f->Get("muonAnalyzer/MuonTree");
  // Int_t nMu = 0;
  // std::vector<float>* muPt = nullptr;
  // std::vector<float>* muEta = nullptr;
  // if (tMu) {
  //   tMu->SetBranchAddress("nMu",  &nMu);
  //   tMu->SetBranchAddress("muPt", &muPt);
  //   tMu->SetBranchAddress("muEta",&muEta);
  // }


  TH1F *hZDCPlus = new TH1F("hZDCPlus", "ZDC Plus Energy", 1000, 0, 100000);
  TH1F *hZDCMinus = new TH1F("hZDCMinus", "ZDC Minus Energy", 1000, 0, 100000);

  Long64_t n = tEvt->GetEntries();
  if (maxEvents > 0 && maxEvents < n)
    n = maxEvents;

  for (Long64_t i = 0; i < n; ++i) {
    tEvt->GetEntry(i);
    tHLT->GetEntry(i);
    tzdc->GetEntry(i);
    // if (tHLT) tHLT->GetEntry(i);
    // if (tMu)  tMu->GetEntry(i);

    // Do your work here; print first few for sanity
    bool isL1ZDCOR =
        HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v || HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v;
    bool isL1ZB =
        HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v || HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v;
    if (!(isL1ZB))
      continue;
    if (debug) {
      std::cout << "HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v: " << HLT_HIUPC_ZDC1nOR_MaxPixelCluster10000_v
                << ", HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v: "
                << HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v << std::endl;
      std::cout << "HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v: " << HLT_HIUPC_ZeroBias_MaxPixelCluster10000_v
                << ", HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v: "
                << HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v << std::endl;

      std::cout << "isL1ZDCOR: " << isL1ZDCOR << ", isL1ZB: " << isL1ZB << std::endl;
      std::cout << "sumPlus: " << sumPlus << ", sumMinus: " << sumMinus << std::endl;
    }
    hZDCPlus->Fill(sumPlus);
    hZDCMinus->Fill(sumMinus);
  }

  TFile *outF = TFile::Open("zdc_energy_histograms.root", "RECREATE");
  hZDCPlus->Write();
  hZDCMinus->Write();
  outF->Close();

  f->Close();
}

