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
#include "/home/xirong/MITHIGAnalysis2024/CommonCode/include/MITHIG_CMSStyle.h"


void plot() {

  TFile *f = TFile::Open("output.root");
  if (!f || f->IsZombie()) {
    std::cerr << "Could not open file!\n";
    return;
  }

  TH1F *hNtrNoPPS = (TH1F *)f->Get("hNtrNoPPS");
  TH1F *hNtrPPS = (TH1F *)f->Get("hNtrPPS");

  TCanvas *cChargedTracks = new TCanvas("cChargedTracks", "cChargedTracks", 1000, 850);

  TPad* padChargedTracks = (TPad*) cChargedTracks->GetPad(0);
  padChargedTracks->cd();

  SetTDRStyle();

  cChargedTracks->SetLogy();
  hNtrNoPPS->Rebin(4);
  hNtrPPS->Rebin(4);
  hNtrNoPPS->Scale(1. / hNtrNoPPS->Integral());
  hNtrNoPPS->SetLineColor(kRed);
  hNtrPPS->Scale(1. / hNtrPPS->Integral());
  hNtrPPS->SetLineColor(kBlue);
  hNtrNoPPS->GetXaxis()->SetTitle("Charged hadron multiplicity, #eta|< 2.4");
  hNtrNoPPS->GetXaxis()->SetRangeUser(0, 300);
  hNtrPPS->GetXaxis()->SetRangeUser(0, 300);
  hNtrNoPPS->SetMaximum(0.5);
  hNtrNoPPS->Draw("same");
  hNtrPPS->Draw("same");

  AddCMSHeader(padChargedTracks);
  AddUPCHeader(padChargedTracks,"5.36TeV","pO");
  padChargedTracks->Update();

  TLegend *legend = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.035);
  legend->AddEntry(hNtrNoPPS, "No req. on PPS", "l");
  legend->AddEntry(hNtrPPS, "At least 1 proton in PPS", "l");
  legend->Draw();
  cChargedTracks->SaveAs("cChargedTracks.png");
}
