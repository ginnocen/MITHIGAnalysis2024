#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TTree.h>
#include <TString.h>
#include <iostream>
#include <TCut.h>

void macroValidation() {

  TFile *fin = new TFile("SkimMCFiltered_OnlyEventswithMuMuTaggedJets.root");
  fin->ls();
  TTree *Tree = (TTree *)fin->Get("Tree");
  const int nClasses = 4;
  TH1F *hMass[nClasses];
  TH1F *hLogSqrtmuDiDxy1Dxy2[nClasses];

  TString var[nClasses] = {"1",
                           "(MJTNcHad==0&&MJTNbHad==0)",
                           "((MJTNcHad==2)&&(MJTNbHad==0))",
                           "((MJTNcHad==2)&&(MJTNbHad==2))"
                          };
  TString classNames[nClasses] = {
                                  "all",
                                  "0c0b",
                                  "2c0b",
                                  "2c2b"
                                 };
  //TCut jetCut = "JetPT>100 && JetPT<140 && abs(JetEta)<2.0 && fabs(muDR)<0.15";
  TCut jetCut = "JetPT>100 && abs(JetEta)<2.0 && muPt1 > 4 && muPt2 > 3.";
  for (int i = 0; i < nClasses; i++) {
    hMass[i] = new TH1F(Form("hMass_%s", classNames[i].Data()), Form(";Mass; Entries"), 50, 0.,10.);
    hLogSqrtmuDiDxy1Dxy2[i] = new TH1F(Form("hLogSqrtmuDiDxy1Dxy2_%s", classNames[i].Data()), Form(";LogSqrtmuDiDxy1Dxy2; Entries"), 24, -6, 0);
    hMass[i]->Sumw2();
    hLogSqrtmuDiDxy1Dxy2[i]->Sumw2();
    Tree->Draw(Form("mumuMass>>hMass_%s", classNames[i].Data()), var[i] && jetCut);
    Tree->Draw(Form("log(sqrt(muDiDxy1Dxy2))>>hLogSqrtmuDiDxy1Dxy2_%s", classNames[i].Data()), var[i] && jetCut);
  }
  hMass[0]->SetMinimum(0.);
  hMass[0]->SetMaximum(hMass[0]->GetMaximum() * 2);
  hLogSqrtmuDiDxy1Dxy2[0]->SetMinimum(0.);
  hLogSqrtmuDiDxy1Dxy2[0]->SetMaximum(hLogSqrtmuDiDxy1Dxy2[0]->GetMaximum() * 2);
  
  TFile *fout = new TFile("histograms.root", "RECREATE"); 
  fout->cd();

  TCanvas *cMass = new TCanvas("cMass", "cMass", 1600, 600);
  TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
  for (int i = 0; i < nClasses; i++) {
    hMass[i]->SetLineColor(i + 1);
    hMass[i]->Write();
    if (i == 0) {
      hMass[i]->Draw();
    } else {
      hMass[i]->Draw("same");
    }
    leg->AddEntry(hMass[i], classNames[i], "l");
  }
  leg->Draw();
  

  TCanvas *cLogSqrtmuDiDxy1Dxy2 = new TCanvas("cLogSqrtmuDiDxy1Dxy2", "cLogSqrtmuDiDxy1Dxy2", 600, 600);
  for (int i = 0; i < nClasses; i++) {
    hLogSqrtmuDiDxy1Dxy2[i]->Write();
    hLogSqrtmuDiDxy1Dxy2[i]->SetLineColor(i + 1);
    if (i == 0) {
      hLogSqrtmuDiDxy1Dxy2[i]->Draw();
    } else {
      hLogSqrtmuDiDxy1Dxy2[i]->Draw("same");
    }
    leg->AddEntry(hLogSqrtmuDiDxy1Dxy2[i], classNames[i], "l");
  }
  fout->Write();
}

