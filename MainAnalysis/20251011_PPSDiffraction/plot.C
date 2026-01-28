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

void plot() {

  TFile *f = TFile::Open("output.root");
  if (!f || f->IsZombie()) {
    std::cerr << "Could not open file!\n";
    return;
  }

  TH1F *hNtrNoCond = (TH1F *)f->Get("hNtrNoCond");
  TH1F *hNtrPPS = (TH1F *)f->Get("hNtrPPS");

  TCanvas *cChargedTracks = new TCanvas("cChargedTracks", "cChargedTracks", 1000, 850);
  cChargedTracks->SetLogy();
  hNtrNoCond->Rebin(4);
  hNtrPPS->Rebin(4);
  hNtrNoCond->Scale(1. / hNtrNoCond->Integral());
  hNtrNoCond->SetLineColor(kRed);
  hNtrPPS->Scale(1. / hNtrPPS->Integral());
  hNtrPPS->SetLineColor(kBlue);
  hNtrNoCond->GetXaxis()->SetTitle("Charged hadron multiplicity, #eta|< 2.4");
  hNtrNoCond->GetXaxis()->SetRangeUser(0, 300);
  hNtrPPS->GetXaxis()->SetRangeUser(0, 300);
  hNtrNoCond->SetMaximum(0.5);
  hNtrNoCond->Draw("same");
  hNtrPPS->Draw("same");
  TLegend *legend = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.035);
  legend->AddEntry(hNtrNoCond, "No requirements", "l");
  legend->AddEntry(hNtrPPS, "At least 1 proton in PPS", "l");
  legend->Draw();
  cChargedTracks->SaveAs("cChargedTracks.png");

  TH1F *hFSC2bottomM_adcPPS = (TH1F *)f->Get("hFSC2bottomM_adcPPS");
  TH1F *hFSC2bottomM_adcNoCond = (TH1F *)f->Get("hFSC2bottomM_adcNoCond");

  TCanvas *cFSC2bottomM = new TCanvas("cFSC2bottomM", "cFSC2bottomM", 1000, 850);
  cFSC2bottomM->SetLogy();
  hFSC2bottomM_adcPPS->Rebin(4);
  hFSC2bottomM_adcNoCond->Rebin(4);
  hFSC2bottomM_adcPPS->Scale(1. / hFSC2bottomM_adcPPS->Integral());
  hFSC2bottomM_adcPPS->SetLineColor(kBlue);
  hFSC2bottomM_adcNoCond->Scale(1. / hFSC2bottomM_adcNoCond->Integral());
  hFSC2bottomM_adcNoCond->SetLineColor(kRed);
  hFSC2bottomM_adcPPS->GetXaxis()->SetTitle("FSC2 bottom ADC");
  hFSC2bottomM_adcPPS->GetXaxis()->SetRangeUser(0, 2000);
  hFSC2bottomM_adcNoCond->GetXaxis()->SetRangeUser(0, 2000);
  hFSC2bottomM_adcPPS->SetMaximum(0.5);
  hFSC2bottomM_adcPPS->Draw("same");
  hFSC2bottomM_adcNoCond->Draw("same");
  TLegend *legend2 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend2->SetBorderSize(0);
  legend2->SetFillStyle(0);
  legend2->SetTextSize(0.035);
  legend2->AddEntry(hFSC2bottomM_adcPPS, "At least 1 proton in PPS", "l");
  legend2->AddEntry(hFSC2bottomM_adcNoCond, "No requirements", "l");
  legend2->Draw();
  cFSC2bottomM->SaveAs("cFSC2bottomM.png");

  TH1F *hFSC2bottomM_chargefCNoCond = (TH1F *)f->Get("hFSC2bottomM_chargefCNoCond");
  TH1F *hFSC2bottomM_chargefCPPS = (TH1F *)f->Get("hFSC2bottomM_chargefCPPS");

  TCanvas *cFSC2bottomM_chargefC = new TCanvas("cFSC2bottomM_chargefC", "cFSC2bottomM_chargefC", 1000, 850);
  cFSC2bottomM_chargefC->SetLogy();
  hFSC2bottomM_chargefCPPS->GetXaxis()->SetRangeUser(0, 400000);
  hFSC2bottomM_chargefCNoCond->GetXaxis()->SetRangeUser(0, 400000);
  hFSC2bottomM_chargefCPPS->Rebin(10);
  hFSC2bottomM_chargefCNoCond->Rebin(10);
  hFSC2bottomM_chargefCPPS->Scale(1. / hFSC2bottomM_chargefCPPS->Integral());
  hFSC2bottomM_chargefCPPS->SetLineColor(kBlue);
  hFSC2bottomM_chargefCNoCond->Scale(1. / hFSC2bottomM_chargefCNoCond->Integral());
  hFSC2bottomM_chargefCNoCond->SetLineColor(kRed);
  hFSC2bottomM_chargefCPPS->GetXaxis()->SetTitle("FSC2 bottom charge [fC]");

  hFSC2bottomM_chargefCPPS->SetMaximum(4.0);
  hFSC2bottomM_chargefCPPS->Draw();
  hFSC2bottomM_chargefCNoCond->Draw("same");
  TLegend *legend3 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend3->SetBorderSize(0);
  legend3->SetFillStyle(0);
  legend3->SetTextSize(0.035);
  legend3->AddEntry(hFSC2bottomM_chargefCPPS, "At least 1 proton in PPS", "l");
  legend3->AddEntry(hFSC2bottomM_chargefCNoCond, "No requirements", "l");
  legend3->Draw();
  cFSC2bottomM_chargefC->SaveAs("cFSC2bottomM_chargefC.png");
}
