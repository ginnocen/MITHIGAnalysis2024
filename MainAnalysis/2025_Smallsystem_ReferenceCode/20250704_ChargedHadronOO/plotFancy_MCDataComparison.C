// run with
// root -l -b -q PlottingExample.C
#include <filesystem>
#include <iostream>
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "include/MITHIG_CMSStyle.h"

// This is only to create sample hists for plotting
void FillExampleHists(
  TH1D* h1,
  TH2D* h2
) {
  h1->Sumw2();
  h2->Sumw2();
  TRandom rand;
  for (int i = 0; i < 5000; ++i) {
    double x = rand.Gaus(0, 1);
    double y = rand.Gaus(0, 1);
    h1->Fill(x);
    h2->Fill(x, y);
  }
}

void divideByWidth(TH1* hist) {
  if (!hist) {
    std::cerr << "Error: Null histogram pointer passed to divideByWidth function." << std::endl;
    return;
  }

  int nBins = hist->GetNbinsX();
  for (int i = 1; i <= nBins; ++i) {
    double binContent = hist->GetBinContent(i);
    double binError = hist->GetBinError(i);
    double binWidth = hist->GetBinWidth(i);

    if (binWidth != 0) {
      hist->SetBinContent(i, binContent / binWidth);
      hist->SetBinError(i, binError / binWidth);
    } else {
      std::cerr << "Warning: Bin width is zero for bin " << i << ". Skipping division for this bin." << std::endl;
    }
  }
}

void LabelBinContent(TH1* hist) {
  for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
    double x = hist->GetBinCenter(bin);
    double y = hist->GetBinContent(bin);
    double y_offset = (y > 0) ? y + 0.03 * (hist->GetMaximum() - hist->GetMinimum()) : 0.03 * (hist->GetMaximum() - hist->GetMinimum());
    TLatex latex;
    latex.SetTextAlign(22);
    latex.SetTextSize(0.027);
    latex.DrawLatex(x, y_offset, Form("%.0f", y));
  }
}

void plotFancy_MCDataComparison() {
  // Make output directory and simple hists for examples
  system("mkdir -p ./plots");

  TFile* fin = TFile::Open("output/20250705_OO_394153_FIRSTLOOK_only25_eventSel_min04_HFAND14_HFAND12.root", "READ");
  TFile* fin_mc = TFile::Open("output/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518_eventSel_min04_HFAND14_HFAND12.root", "READ");

  string output_prefix = "plots/fancy_min04_HFAND14_HFAND12";

  TH1D *hTrkPt = (TH1D*)fin->Get("hTrkPt");
  TH1D *hNEvtPassCuts = (TH1D*)fin->Get("hNEvtPassCuts");
  TH1D *hTrkEta = (TH1D*)fin->Get("hTrkEta");

  TH2D *hZDCPlusMinus = (TH2D*)fin->Get("hZDCPlusMinus");
  TH1D *hZDCPlus = (TH1D*)hZDCPlusMinus->ProjectionX("hZDCPlus");
  TH1D *hZDCMinus = (TH1D*)hZDCPlusMinus->ProjectionY("hZDCMinus");

  float MC_scale = 30887./892630; // Scale factor for MC histograms

  // Scale all MC histograms by MC_scale
  // (Do this immediately after loading them)
  // all except, N pass evt cuts
  TH1D *hTrkPt_MC = (TH1D*)fin_mc->Get("hTrkPt");
  if (hTrkPt_MC) hTrkPt_MC->Scale(MC_scale);

  TH1D *hTrkEta_MC = (TH1D*)fin_mc->Get("hTrkEta");
  if (hTrkEta_MC) hTrkEta_MC->Scale(MC_scale);

  TH1D *hNEvtPassCuts_MC = (TH1D*)fin_mc->Get("hNEvtPassCuts");

  TH2D *hZDCPlusMinus_MC = (TH2D*)fin_mc->Get("hZDCPlusMinus");
  if (hZDCPlusMinus_MC) hZDCPlusMinus_MC->Scale(MC_scale);

  TH1D *hMult_MC = (TH1D*)fin_mc->Get("hMult");
  if (hMult_MC) hMult_MC->Scale(MC_scale);
  
  // ===========================================================================
  // EXAMPLE 1: Styling a Single Canvas ----------------------------------------
  TCanvas* ex1Canvas = new TCanvas("ex1Canvas", "", 800, 600);

  // Get the canvas pad to pass to other functions
  TPad* ex1Pad = (TPad*) ex1Canvas->GetPad(0);
  ex1Pad->cd();
  ex1Pad->SetLogy();

  // >>> Apply the CMS TDR style <<<
  SetTDRStyle();

  divideByWidth(hTrkPt);
  divideByWidth(hTrkPt_MC);

  // Draw the TH1 as normal.
  hTrkPt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hTrkPt->GetYaxis()->SetTitle("dN/dp_{T} (event normalized)");
  hTrkPt->GetXaxis()->SetRangeUser(0.5, 25);
  hTrkPt->GetYaxis()->SetRangeUser(1, 2e7);

  hTrkPt->SetMarkerColor(cmsBlue);
  hTrkPt->SetMarkerStyle(mCircleFill);
  hTrkPt->SetLineColor(cmsRed);
  hTrkPt->SetLineWidth(3);
  hTrkPt->Draw();

  // MC styling
  hTrkPt_MC->SetLineColor(kOrange+1);
  hTrkPt_MC->SetLineStyle(2);
  hTrkPt_MC->SetLineWidth(2);
  hTrkPt_MC->Draw("HIST SAME");

  TLegend* leg1 = new TLegend(0.55, 0.70, 0.85, 0.82);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->SetTextFont(42);
  leg1->SetTextSize(0.035);
  leg1->AddEntry(hTrkPt, "OO data Run 394153", "pl");
  leg1->AddEntry(hTrkPt_MC, "OO HIJING MC", "l");
  leg1->Draw();

  AddCMSHeader(
    ex1Pad,
    "Internal",
    false
  );
  AddUPCHeader(ex1Pad, "9.6 TeV", "Run 394153 OO");
  ex1Pad->Update();

  ex1Canvas->SaveAs(Form("%s-pT.pdf", output_prefix.c_str()));


  // ===========================================================================
  TCanvas* ex2Canvas = new TCanvas("ex2Canvas", "", 800, 600);

  TPad* ex2Pad = (TPad*) ex2Canvas->GetPad(0);
  ex2Pad->cd();

  SetTDRStyle();

  hNEvtPassCuts->GetYaxis()->SetTitle("Counts");
  hNEvtPassCuts->GetYaxis()->SetRangeUser(0, 1.5e6);
  hNEvtPassCuts->SetLineColor(cmsRed);
  hNEvtPassCuts->SetLineWidth(3);
  hNEvtPassCuts->Draw("HIST TEXT0");

  // MC styling
  hNEvtPassCuts_MC->SetLineColor(kOrange+1);
  hNEvtPassCuts_MC->SetLineStyle(2);
  hNEvtPassCuts_MC->SetLineWidth(2);
  hNEvtPassCuts_MC->Draw("HIST SAME TEXT0");

  TLegend* leg2 = new TLegend(0.55, 0.70, 0.85, 0.82);
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->SetTextFont(42);
  leg2->SetTextSize(0.035);
  leg2->AddEntry(hNEvtPassCuts, "OO data Run 394153", "l");
  leg2->AddEntry(hNEvtPassCuts_MC, "OO HIJING MC", "l");
  leg2->Draw();

  AddCMSHeader(
    ex2Pad,
    "Internal",
    true
  );
  AddUPCHeader(ex2Pad, "9.6 TeV", "Run 394153 OO");
  ex2Pad->Update();

  ex2Canvas->SaveAs(Form("%s-NEvtPass.pdf", output_prefix.c_str()));

  // ===========================================================================
  TCanvas* ex3Canvas = new TCanvas("ex3Canvas", "", 800, 600);
  TPad* ex3Pad = (TPad*) ex3Canvas->GetPad(0);
  ex3Pad->cd();

  SetTDRStyle();

  divideByWidth(hTrkEta);
  divideByWidth(hTrkEta_MC);

  hTrkEta->GetXaxis()->SetTitle("#eta");
  hTrkEta->GetYaxis()->SetTitle("dN/d#eta (event normalized)");
  hTrkEta->GetXaxis()->SetRangeUser(-2.4, 2.4);
  hTrkEta->GetYaxis()->SetRangeUser(0, 1250e3);

  hTrkEta->SetMarkerColor(cmsRed);
  hTrkEta->SetMarkerStyle(mCircleFill);
  hTrkEta->SetLineColor(cmsBlue);
  hTrkEta->SetLineWidth(3);
  hTrkEta->Draw();

  // MC styling
  hTrkEta_MC->SetLineColor(kOrange+1);
  hTrkEta_MC->SetLineStyle(2);
  hTrkEta_MC->SetLineWidth(2);
  hTrkEta_MC->Draw("HIST SAME");

  TLegend* leg3 = new TLegend(0.55, 0.70, 0.85, 0.82);
  leg3->SetBorderSize(0);
  leg3->SetFillStyle(0);
  leg3->SetTextFont(42);
  leg3->SetTextSize(0.035);
  leg3->AddEntry(hTrkEta, "OO data Run 394153", "pl");
  leg3->AddEntry(hTrkEta_MC, "OO HIJING MC", "l");
  leg3->Draw();

  AddCMSHeader(
    ex3Pad,
    "Internal",
    true
  );
  AddUPCHeader(ex3Pad, "9.6 TeV", "Run 394153 OO");
  ex3Pad->Update();

  ex3Canvas->SaveAs(Form("%s-eta.pdf", output_prefix.c_str()));

  // ===========================================================================
  // Plot multiplicity distribution (assuming hMult exists in your ROOT file)
  TH1D *hMult = (TH1D*)fin->Get("hMult");

  TCanvas* ex5Canvas = new TCanvas("ex5Canvas", "", 800, 600);
  TPad* ex5Pad = (TPad*) ex5Canvas->GetPad(0);
  ex5Pad->cd();
  ex5Pad->SetLogy();

  SetTDRStyle();

  hMult->GetXaxis()->SetTitle("Track Multiplicity");
  hMult->GetYaxis()->SetTitle("Counts (event normalized)");
  hMult->GetXaxis()->SetRangeUser(0, 700);
  hMult->GetYaxis()->SetRangeUser(1, 5e4);
  hMult->SetMarkerColor(cmsRed);
  hMult->SetMarkerStyle(mCircleFill);
  hMult->SetLineColor(cmsBlue);
  hMult->SetLineWidth(3);
  hMult->Draw();

  // MC styling
  hMult_MC->SetLineColor(kOrange+1);
  hMult_MC->SetLineStyle(2);
  hMult_MC->SetLineWidth(2);
  hMult_MC->Draw("HIST SAME");

  TLegend* leg5 = new TLegend(0.55, 0.70, 0.85, 0.82);
  leg5->SetBorderSize(0);
  leg5->SetFillStyle(0);
  leg5->SetTextFont(42);
  leg5->SetTextSize(0.035);
  leg5->AddEntry(hMult, "OO data Run 394153", "pl");
  leg5->AddEntry(hMult_MC, "OO HIJING MC", "l");
  leg5->Draw();

  AddCMSHeader(
    ex5Pad,
    "Internal",
    true
  );
  AddUPCHeader(ex5Pad, "9.6 TeV", "Run 394153 OO");
  ex5Pad->Update();

  ex5Canvas->SaveAs(Form("%s-multiplicity.pdf", output_prefix.c_str()));

}
