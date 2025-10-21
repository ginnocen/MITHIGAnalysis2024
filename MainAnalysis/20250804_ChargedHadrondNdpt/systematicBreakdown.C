 #include "MITHIG_CMSStyle.h"

void systematicBreakdown() {
  // Load the nominal histogram
  SetTDRStyle();
  TFile* fNom = TFile::Open("NeNe_Aug2nd_2025v2.root");
  TH1* hNom = (TH1*)fNom->Get("hTrkPt");
  hNom->SetDirectory(0);
  fNom->Close();

  // Canvas setup
  TCanvas* c = new TCanvas("c", "Relative Differences", 900, 600);
  gStyle->SetOptStat(0);
  c->SetGrid();

  TLegend* leg = new TLegend(0.45, 0.63, 0.88, 0.88);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.035);
  
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.055);
    latex.DrawLatex(0.55, 0.95, "NeNe #bf{1 nb^{-1} (5.36 TeV)}");
    latex.DrawLatex(0.11, 0.95, "CMS #bf{#it{Preliminary}}");

  // Flat ±5% luminosity uncertainty band
  auto hLumBand = (TH1*)hNom->Clone("hLuminosityBand");
  for (int i = 1; i <= hLumBand->GetNbinsX(); ++i) {
    hLumBand->SetBinContent(i, 0);
    hLumBand->SetBinError(i, 0.05);
  }
  hLumBand->SetFillColorAlpha(kGray + 1, 0.4);
  hLumBand->SetLineColor(0);
  hLumBand->SetMarkerSize(0);
  hLumBand->SetTitle("Relative Differences to Nominal");
  hLumBand->GetYaxis()->SetTitle("Relative difference to Nominal");
  hLumBand->GetXaxis()->SetTitle("p_{T}^{ch} (GeV/c)");
  hLumBand->GetYaxis()->SetRangeUser(-0.2, 0.2);
  hLumBand->Draw("E2");
  leg->AddEntry(hLumBand, "Luminosity (placeholder)", "f");

  // Symmetrized statistical uncertainty band
  auto hStatBand = (TH1*)hNom->Clone("hStatBand");
  for (int i = 1; i <= hStatBand->GetNbinsX(); ++i) {
    double content = hNom->GetBinContent(i);
    double err = hNom->GetBinError(i);
    double relErr = (content != 0) ? err / content : 0;
    hStatBand->SetBinContent(i, 0);
    hStatBand->SetBinError(i, relErr);
  }
  hStatBand->SetFillColorAlpha(kBlue - 7, 0.3);
  hStatBand->SetLineColor(0);
  hStatBand->Draw("E2 same");
  leg->AddEntry(hStatBand, "Statistical uncertainty (Nominal)", "f");

  // List of variant files
  std::vector<std::pair<TString, TString>> files = {
    {"MergedOutputNeNeTight.root", "Tight track selection"},
    {"MergedOutputNeNePPref.root", "Species-by-species, ppRef"},
    {"MergedOutputNeNeLoose.root", "Loose track selection"},
    {"MergedOutputDNdeta100.root", "Species-by-species, dN/d#eta = 100"}
  };
  

    TLatex latex2;
    latex2.SetNDC();
    latex2.SetTextSize(0.055);
    latex2.DrawLatex(0.55, 0.95, "NeNe #bf{1 nb^{-1} (5.36 TeV)}");
    latex2.DrawLatex(0.11, 0.95, "CMS #bf{#it{Preliminary}}");

  // Plot relative differences for each
  int colorIndex = 1;
  for (const auto& [fileName, label] : files) {
    TFile* f = TFile::Open(fileName);
    TH1* h = (TH1*)f->Get("hTrkPt");
    h->SetDirectory(0);
    f->Close();

    TH1* hRatio = (TH1*)h->Clone("hRatio_" + fileName);
    hRatio->Add(hNom, -1);
    hRatio->Divide(hNom);

    hRatio->SetLineColor(colorIndex);
    hRatio->SetMarkerColor(colorIndex);
    hRatio->SetMarkerStyle(20 + colorIndex);
    hRatio->SetLineWidth(2);
    hRatio->Draw("hist same");

    leg->AddEntry(hRatio, label, "l");
    colorIndex++;
  }

  leg->Draw();
  c->SaveAs("allRelativeUncertainties_SymmetricStat_hTrkPt.pdf");
}

