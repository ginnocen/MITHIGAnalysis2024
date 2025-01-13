#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"

//int main() {
int cpptest() {
  std::cout<<"--- Starting cpptest.cc"<<std::endl;
  
  TFile* fin = TFile::Open(
    "root://xrootd.cmsaf.mit.edu//store/user/jdlang/run3_2024PromptReco/Run3UPC2024_PromptReco_388000_HIForward0/HIForward0/crab_Run3UPC2024_PromptReco_388000_HIForward0/241119_030005/0000/HiForestMiniAOD_1.root"
  );
  if (!fin || fin->IsZombie()) {
    std::cerr << "Error: Could not open file!" << std::endl;
    return 1;
  }
  
  TTree* dtree = (TTree*) fin->Get("Dfinder/ntDkpi");
  if (!dtree) {
    std::cerr << "Error: Could not retrieve tree!" << std::endl;
    fin->Close();
    return 1;
  }

  TH1D* hDpt = new TH1D(
    "hDpt",
    "D^{0} p_{T}; p_{T} [GeV/c]; counts",
    10, 0., 5.
  );
  int Dsize;
  float* Dpt = new float[1000];
  dtree->SetBranchAddress("Dsize", &Dsize);
  dtree->SetBranchAddress("Dpt", Dpt);
  for (int i = 0; i < dtree->GetEntries(); i++) {
    if (i % 100 == 0) std::cout<<"Reading event "<<i<<std::endl;
    dtree->GetEntry(i);
    if (Dsize <= 0) continue;
    for (int d = 0; d < TMath::Min(Dsize, 100); d++) {
      hDpt->Fill(Dpt[d]);
    }
  }
  TFile* fout = new TFile(
    "cpptest_output.root",
    "recreate"
  );
  fout->cd();
  hDpt->Write();
  fin->Close();
  fout->Close();
  return 0;
}
