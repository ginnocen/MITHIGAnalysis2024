#include "TROOT.h"

void ConvertGraph(
  TFile* fin,
  TFile* fout,
  TString graphName
) {
  TGraphAsymmErrors* gAE = (TGraphAsymmErrors*) fin->Get(graphName);
  gAE->SetName(graphName + "_AE");
  TGraphErrors* gE = new TGraphErrors(gAE->GetN());
  for (int i=1; i < 5; i++) {
    gE->AddPointError(
      gAE->GetPointX(i),
      gAE->GetPointY(i),
      gAE->GetErrorXhigh(i),
      gAE->GetErrorYhigh(i)
    );
  }
  fout->cd();
  gE->SetName(graphName);
  gE->Write();
}

void ConvertTGraphAsymmErrors() {
  TFile* fin = TFile::Open("Theory_RAAvsA/theory_RAAvsA_Bayesian-qHat.root", "READ");
  TFile* fout = TFile::Open("Theory_RAAvsA/theory_RAAvsA_Bayesian-qHat_Fix.root", "RECREATE");
  
  ConvertGraph(fin, fout, "gRAA_pT_7p2_9p6");
  ConvertGraph(fin, fout, "gRAA_pT_9p6_12");
  ConvertGraph(fin, fout, "gRAA_pT_12_14p4");
  ConvertGraph(fin, fout, "gRAA_pT_14p4_19p2");
  ConvertGraph(fin, fout, "gRAA_pT_19p2_24");
  ConvertGraph(fin, fout, "gRAA_pT_24_28p8");
  ConvertGraph(fin, fout, "gRAA_pT_28p8_35p2");
  ConvertGraph(fin, fout, "gRAA_pT_35p2_48");
  ConvertGraph(fin, fout, "gRAA_pT_48_73p6");
  ConvertGraph(fin, fout, "gRAA_pT_73p6_103p6");

  fout->Close();
  fin->Close();
}
