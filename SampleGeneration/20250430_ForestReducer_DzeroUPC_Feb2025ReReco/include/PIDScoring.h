#include <vector>
#include <filesystem>

#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TMath.h"

float GetPIDScore(
  float p,
  float dedx,
  TF1* fCenter,
  TF1* fSigmaLo,
  TF1* fSigmaHi,
  bool logdedx = true
) {
  float PIDScore;
  if (logdedx) dedx = TMath::Log(dedx);
  float dedxCenter = fCenter->Eval(p);
  if (dedx < dedxCenter)
    PIDScore = (dedx - dedxCenter) / (dedxCenter - fSigmaLo->Eval(p));
  else
    PIDScore = (dedx - dedxCenter) / (fSigmaHi->Eval(p) - dedxCenter);
  return PIDScore;
}

void SavePIDRoot(
  TF1* fdedxPionCenter,
  TF1* fdedxPionSigmaLo,
  TF1* fdedxPionSigmaHi,
  TF1* fdedxKaonCenter,
  TF1* fdedxKaonSigmaLo,
  TF1* fdedxKaonSigmaHi,
  TF1* fdedxProtCenter,
  TF1* fdedxProtSigmaLo,
  TF1* fdedxProtSigmaHi,
  TString output = "../../CommonCode/root/DzeroUPC_dedxMap.root"
) {
  TFile* fout = new TFile(output, "RECREATE");
  fout->cd();
  fdedxPionCenter->SetName("fdedxPionCenter");
  fdedxPionSigmaLo->SetName("fdedxPionSigmaLo");
  fdedxPionSigmaHi->SetName("fdedxPionSigmaHi");
  fdedxKaonCenter->SetName("fdedxKaonCenter");
  fdedxKaonSigmaLo->SetName("fdedxKaonSigmaLo");
  fdedxKaonSigmaHi->SetName("fdedxKaonSigmaHi");
  fdedxProtCenter->SetName("fdedxProtCenter");
  fdedxProtSigmaLo->SetName("fdedxProtSigmaLo");
  fdedxProtSigmaHi->SetName("fdedxProtSigmaHi");
  fdedxPionCenter->Write();
  fdedxPionSigmaLo->Write();
  fdedxPionSigmaHi->Write();
  fdedxKaonCenter->Write();
  fdedxKaonSigmaLo->Write();
  fdedxKaonSigmaHi->Write();
  fdedxProtCenter->Write();
  fdedxProtSigmaLo->Write();
  fdedxProtSigmaHi->Write();
}

std::vector<TF1*> ImportPIDRoot(
  TString input = "../../CommonCode/root/DzeroUPC_dedxMap.root"
) {
  TFile* dedxInput = new TFile(input, "READ");
  TF1* fdedxPionCenter  = (TF1*) dedxInput->Get("fdedxPionCenter");
  TF1* fdedxPionSigmaLo = (TF1*) dedxInput->Get("fdedxPionSigmaLo");
  TF1* fdedxPionSigmaHi = (TF1*) dedxInput->Get("fdedxPionSigmaHi");
  TF1* fdedxKaonCenter  = (TF1*) dedxInput->Get("fdedxKaonCenter");
  TF1* fdedxKaonSigmaLo = (TF1*) dedxInput->Get("fdedxKaonSigmaLo");
  TF1* fdedxKaonSigmaHi = (TF1*) dedxInput->Get("fdedxKaonSigmaHi");
  TF1* fdedxProtCenter  = (TF1*) dedxInput->Get("fdedxProtCenter");
  TF1* fdedxProtSigmaLo = (TF1*) dedxInput->Get("fdedxProtSigmaLo");
  TF1* fdedxProtSigmaHi = (TF1*) dedxInput->Get("fdedxProtSigmaHi");
  std::vector<TF1*> dedxFunctions = {
    fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi,
    fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi,
    fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi
  };
  dedxInput->Close();
  delete dedxInput;
  return dedxFunctions;
}

// Run this using root to add PID branches to skimmed file tree:
// root -x -q PIDScoring.h
void PIDScoring() {
  std::cout<<"Starting..."<<std::endl;
  
  // Set this to the ORIGINAL skim file without PID:
  TString fileSourcePath;
  fileSourcePath = "/data00/jdlang/UPCD0LowPtAnalysis/SkimsMC/20250619_Skim_2023MC_Feb2025ReReco_PhotonBeamB_forcedDecays.root";
  // Set this to the path for the copy of the above file that will include PID:
  TString filePIDPath;
  filePIDPath = "/data00/jdlang/UPCD0LowPtAnalysis/SkimsMC/20250619_Skim_2023MC_Feb2025ReReco_PhotonBeamB_forcedDecays_PID.root";
  
  std::cout<<"Getting root files..."<<std::endl;
  system(Form("rm %s; wait", filePIDPath.Data()));
  system(Form("cp %s %s; wait", fileSourcePath.Data(), filePIDPath.Data()));
  TFile* fin = new TFile(filePIDPath, "UPDATE");
  TTree* Tree = (TTree*) fin->Get("Tree");
  
  std::cout<<"Getting dE/dx curves"<<std::endl;
  std::vector<TF1*> dedxFunctions = ImportPIDRoot("../../CommonCode/root/DzeroUPC_dedxMap.root");
  TF1* fdedxPionCenter  = dedxFunctions[0];
  TF1* fdedxPionSigmaLo = dedxFunctions[1];
  TF1* fdedxPionSigmaHi = dedxFunctions[2];
  TF1* fdedxKaonCenter  = dedxFunctions[3];
  TF1* fdedxKaonSigmaLo = dedxFunctions[4];
  TF1* fdedxKaonSigmaHi = dedxFunctions[5];
  TF1* fdedxProtCenter  = dedxFunctions[6];
  TF1* fdedxProtSigmaLo = dedxFunctions[7];
  TF1* fdedxProtSigmaHi = dedxFunctions[8];
  fin->cd();

  std::cout<<"Accessing existing branches..."<<std::endl;
  int Dsize;
  std::vector<float>* Dtrk1Pt = nullptr;
  std::vector<float>* Dtrk1Eta = nullptr;
  std::vector<float>* Dtrk1dedx = nullptr;
  std::vector<float>* Dtrk2Pt = nullptr;
  std::vector<float>* Dtrk2Eta = nullptr;
  std::vector<float>* Dtrk2dedx = nullptr;
  Tree->SetBranchAddress("Dsize", &Dsize);
  Tree->SetBranchAddress("Dtrk1Pt",   &Dtrk1Pt);
  Tree->SetBranchAddress("Dtrk1Eta",  &Dtrk1Eta);
  Tree->SetBranchAddress("Dtrk1dedx", &Dtrk1dedx);
  Tree->SetBranchAddress("Dtrk2Pt",   &Dtrk2Pt);
  Tree->SetBranchAddress("Dtrk2Eta",  &Dtrk2Eta);
  Tree->SetBranchAddress("Dtrk2dedx", &Dtrk2dedx);
  
  std::cout<<"Adding new branches..."<<std::endl;
  std::vector<float>* Dtrk1P = new std::vector<float>();
  std::vector<float>* Dtrk1PionScore = new std::vector<float>();
  std::vector<float>* Dtrk1KaonScore = new std::vector<float>();
  std::vector<float>* Dtrk1ProtScore = new std::vector<float>();
  std::vector<float>* Dtrk2P = new std::vector<float>();
  std::vector<float>* Dtrk2PionScore = new std::vector<float>();
  std::vector<float>* Dtrk2KaonScore = new std::vector<float>();
  std::vector<float>* Dtrk2ProtScore = new std::vector<float>();
  TBranch* bDtrk1P          = Tree->Branch("Dtrk1P",          &Dtrk1P);
  TBranch* bDtrk1PionScore  = Tree->Branch("Dtrk1PionScore",  &Dtrk1PionScore);
  TBranch* bDtrk1KaonScore  = Tree->Branch("Dtrk1KaonScore",  &Dtrk1KaonScore);
  TBranch* bDtrk1ProtScore  = Tree->Branch("Dtrk1ProtScore",  &Dtrk1ProtScore);
  TBranch* bDtrk2P          = Tree->Branch("Dtrk2P",          &Dtrk2P);
  TBranch* bDtrk2PionScore  = Tree->Branch("Dtrk2PionScore",  &Dtrk2PionScore);
  TBranch* bDtrk2KaonScore  = Tree->Branch("Dtrk2KaonScore",  &Dtrk2KaonScore);
  TBranch* bDtrk2ProtScore  = Tree->Branch("Dtrk2ProtScore",  &Dtrk2ProtScore);
  
  std::cout<<"Starting loop over entries..."<<std::endl;
  for (Long64_t i = 0; i < Tree->GetEntries(); i++) {
    if (i % 100000 == 0) std::cout<<"Processing entry "<<i<<std::endl;
    Tree->GetEntry(i);
    for (int j = 0; j < Dsize; j++) {
      float p1 = TMath::Abs(Dtrk1Pt->at(j)/(1 - TMath::TanH(Dtrk1Eta->at(j))));
      float dedx1 = Dtrk1dedx->at(j);
      float p2 = TMath::Abs(Dtrk2Pt->at(j)/(1 - TMath::TanH(Dtrk2Eta->at(j))));
      float dedx2 = Dtrk2dedx->at(j);
      Dtrk1P->push_back(p1);
      Dtrk1PionScore->push_back(GetPIDScore(
        p1, dedx1, fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
      Dtrk1KaonScore->push_back(GetPIDScore(
        p1, dedx1, fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
      Dtrk1ProtScore->push_back(GetPIDScore(
        p1, dedx1, fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
      Dtrk2P->push_back(p2);
      Dtrk2PionScore->push_back(GetPIDScore(
        p2, dedx2, fdedxPionCenter, fdedxPionSigmaLo, fdedxPionSigmaHi));
      Dtrk2KaonScore->push_back(GetPIDScore(
        p2, dedx2, fdedxKaonCenter, fdedxKaonSigmaLo, fdedxKaonSigmaHi));
      Dtrk2ProtScore->push_back(GetPIDScore(
        p2, dedx2, fdedxProtCenter, fdedxProtSigmaLo, fdedxProtSigmaHi));
    } // end loop over Dsize
//    if (Dsize > 0) {
      // Fill new branches
      bDtrk1P->Fill();
      bDtrk1PionScore->Fill();
      bDtrk1KaonScore->Fill();
      bDtrk1ProtScore->Fill();
      bDtrk2P->Fill();
      bDtrk2PionScore->Fill();
      bDtrk2KaonScore->Fill();
      bDtrk2ProtScore->Fill();
//    }
    // Clear variables
    Dtrk1P->clear();
    Dtrk1PionScore->clear();
    Dtrk1KaonScore->clear();
    Dtrk1ProtScore->clear();
    Dtrk2P->clear();
    Dtrk2PionScore->clear();
    Dtrk2KaonScore->clear();
    Dtrk2ProtScore->clear();
    Dsize = 0;
  } // end loop over entries
  Tree->Write();
  fin->Close();
  delete fin;
}
