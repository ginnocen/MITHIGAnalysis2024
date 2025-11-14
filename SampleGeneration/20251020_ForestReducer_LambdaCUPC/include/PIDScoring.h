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
