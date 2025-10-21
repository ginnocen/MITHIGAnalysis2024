#include "MITHIG_CMSStyle.h"

void PlotTrkPtVariantsWithRatioToUnweighted_Save(const char *filename = "MergedOutput.root",
                                                 const char *outputImage = "trkPtVariants_ratioToRaw.png",
                                                 const char *outputRoot = "trkPt_outputHistos.root",
                                                 bool logy = false) {
  // Open file
  TFile *file = TFile::Open(filename);
  if (!file || file->IsZombie()) {
    std::cerr << "ERROR: Could not open file: " << filename << std::endl;
    return;
  }

  // Load histograms
  auto get = [&](const char *name) { return dynamic_cast<TH1D *>(file->Get(name)); };
  TH1D *hUnweighted = get("hTrkPtUnweighted");
  TH1D *hNoEvt = get("hTrkPtNoEvt");
  TH1D *hNoTrk = get("hTrkPtNoTrk");
  TH1D *hNoPartSpecies = get("hTrkPtNoPartSpecies");
  TH1D *hFull = get("hTrkPt");

  if (!hUnweighted || !hNoEvt || !hNoTrk || !hNoPartSpecies || !hFull) {
    std::cerr << "ERROR: One or more histograms not found in the file." << std::endl;
    return;
  }

  // Normalize all histograms per bin width
  for (TH1D *h : {hUnweighted, hNoEvt, hNoTrk, hNoPartSpecies, hFull}) {
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
      //double width = h->GetBinWidth(i);
      //h->SetBinContent(i, h->GetBinContent(i) / width);
      //h->SetBinError(i, h->GetBinError(i) / width);
    }
  }

  // Save selected histograms
  TFile *fout = new TFile(outputRoot, "RECREATE");
  hFull->SetName("hTrkPt_Corrected_perBinWidth");
  hUnweighted->SetName("hTrkPtUnweighted_perBinWidth");
  hFull->Write();
  hUnweighted->Write();
  fout->Close();
  std::cout << "Saved histograms to " << outputRoot << std::endl;

  // Create ratio histograms to raw
  auto makeRatio = [&](TH1D *h, const char *newname) {
    TH1D *r = (TH1D *)h->Clone(newname);
    r->SetDirectory(0);
    r->Divide(hUnweighted);
    return r;
  };

  TH1D *rNoEvt = makeRatio(hNoEvt, "rNoEvt");
  TH1D *rNoTrk = makeRatio(hNoTrk, "rNoTrk");
  TH1D *rNoPartSpecies = makeRatio(hNoPartSpecies, "rNoPartSpecies");
  TH1D *rFullRatio = makeRatio(hFull, "rFull");

  // Set styles
  hUnweighted->SetLineColor(kGray + 2);
  hUnweighted->SetLineWidth(2);
  hUnweighted->SetMarkerStyle(20);
  hUnweighted->SetMarkerColor(kGray + 2);
  hNoEvt->SetLineColor(kRed + 1);
  hNoEvt->SetLineWidth(2);
  hNoEvt->SetMarkerStyle(21);
  hNoEvt->SetMarkerColor(kRed + 1);
  hNoTrk->SetLineColor(kOrange + 7);
  hNoTrk->SetLineWidth(2);
  hNoTrk->SetMarkerStyle(22);
  hNoTrk->SetMarkerColor(kOrange + 7);
  hNoPartSpecies->SetLineColor(kGreen + 2);
  hNoPartSpecies->SetLineWidth(2);
  hNoPartSpecies->SetMarkerStyle(23);
  hNoPartSpecies->SetMarkerColor(kGreen + 2);
  hFull->SetLineColor(kBlue + 2);
  hFull->SetLineWidth(2);
  hFull->SetMarkerStyle(24);
  hFull->SetMarkerColor(kBlue + 2);

  rNoEvt->SetLineColor(kRed + 1);
  rNoEvt->SetMarkerStyle(21);
  rNoEvt->SetMarkerColor(kRed + 1);
  rNoTrk->SetLineColor(kOrange + 7);
  rNoTrk->SetMarkerStyle(22);
  rNoTrk->SetMarkerColor(kOrange + 7);
  rNoPartSpecies->SetLineColor(kGreen + 2);
  rNoPartSpecies->SetMarkerStyle(23);
  rNoPartSpecies->SetMarkerColor(kGreen + 2);
  rFullRatio->SetLineColor(kBlue + 2);
  rFullRatio->SetMarkerStyle(24);
  rFullRatio->SetMarkerColor(kBlue + 2);

  // Axis labels
  hUnweighted->GetXaxis()->SetTitle("");
  hUnweighted->GetYaxis()->SetTitle("dN/dp_{T} (per GeV)");

  rNoEvt->GetXaxis()->SetTitle("p_{T} (GeV)");
  rNoEvt->GetYaxis()->SetTitle("Ratio to Raw");

  rNoEvt->SetMinimum(0.0);
  rNoEvt->SetMaximum(2.0);

  rNoEvt->GetYaxis()->SetTitleSize(0.14);
  rNoEvt->GetYaxis()->SetTitleOffset(0.5);
  rNoEvt->GetYaxis()->SetLabelSize(0.07);
  rNoEvt->GetXaxis()->SetLabelSize(0.07);
  rNoEvt->GetXaxis()->SetTitleSize(0.08);
  rNoEvt->GetXaxis()->SetTitleOffset(1.0);

  rNoEvt->GetYaxis()->SetTitleSize(0.05);
  rNoEvt->GetYaxis()->SetTitleOffset(0.8);
  rNoEvt->GetYaxis()->SetLabelSize(0.07);
  rNoEvt->GetXaxis()->SetTitleSize(0.07);
  rNoEvt->GetXaxis()->SetTitleOffset(1.0);
  rNoEvt->GetXaxis()->SetLabelSize(0.07);

  SetTDRStyle();
  // Canvas with two pads
  TCanvas *c = new TCanvas("cTrkPtVariants", "Track pT Variants with Ratio to Raw", 800, 800);
  gStyle->SetOptStat(0);
  TPad *pad1 = new TPad("pad1", "Main pad", 0, 0.32, 1, 1.0);
  TPad *pad2 = new TPad("pad2", "Ratio pad", 0, 0.05, 1, 0.32);
  pad1->SetBottomMargin(0.05);
  pad2->SetTopMargin(0.02);
  pad2->SetBottomMargin(0.32);
  pad1->Draw();
  pad2->Draw();

  // ==== Main plot ====
  pad1->cd();
  AddCMSHeader(pad1);
  if (logy)
    pad1->SetLogy();
  //    pad1->SetGrid();
  SetTDRStyle();

  double maxY = std::max({hUnweighted->GetMaximum(), hNoEvt->GetMaximum(), hNoTrk->GetMaximum(),
                          hNoPartSpecies->GetMaximum(), hFull->GetMaximum()});
  hUnweighted->SetMaximum(1.3 * maxY);
  hUnweighted->GetYaxis()->SetTitleSize(0.05);
  hUnweighted->GetYaxis()->SetTitleOffset(0.75);
  hUnweighted->Draw("HIST E");
  hUnweighted->GetXaxis()->SetLabelSize(0);
  hNoEvt->Draw("HIST E SAME");
  hNoTrk->Draw("HIST E SAME");
  hNoPartSpecies->Draw("HIST E SAME");
  hFull->Draw("HIST E SAME");

  TLegend *leg = new TLegend(0.55, 0.65, 0.88, 0.88);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry(hUnweighted, "Uncorrected p_{T} spectrum", "lep");
  leg->AddEntry(hNoEvt, "Only event weight", "lep");
  leg->AddEntry(hNoTrk, "Only track-by-track", "lep");
  leg->AddEntry(hNoPartSpecies, "Only species-by-species", "lep");
  leg->AddEntry(hFull, "Fully corrected p_{T} spectrum", "lep");
  leg->Draw();

  TLatex latex2;
  latex2.SetNDC();
  latex2.SetTextSize(0.055);
  latex2.DrawLatex(0.55, 0.95, "NeNe #bf{1 nb^{-1} (5.36 TeV)}");
  latex2.DrawLatex(0.11, 0.95, "CMS #bf{#it{Preliminary}}");
  //    latex2.DrawLatex(0.18, 0.85, "OO 5.36 TeV");

  // ==== Ratio plot ====
  pad2->cd();
  SetTDRStyle();
  //    pad2->SetGrid();
  // gStyle->SetTickLength(0.06, "X");
  // gStyle->SetTickLength(0.06, "Y");
  rNoEvt->GetYaxis()->SetTitleSize(0.1);
  rNoEvt->GetYaxis()->SetTitleOffset(0.3);
  rNoEvt->GetYaxis()->SetLabelSize(0.1);
  rNoEvt->GetXaxis()->SetTitleSize(0.14);
  rNoEvt->GetXaxis()->CenterTitle();
  rNoEvt->GetXaxis()->SetTitleOffset(1.0);
  rNoEvt->GetXaxis()->SetLabelSize(0.1);
  rNoEvt->GetXaxis()->SetTickLength(0.1); // Increase from default (~0.03)
  rNoEvt->GetYaxis()->SetTickLength(0.05);
  rNoEvt->GetYaxis()->SetNdivisions(205);
  rNoEvt->Draw("P E");
  rNoTrk->Draw("P E SAME");
  rNoPartSpecies->Draw("P E SAME");
  rFullRatio->Draw("P E SAME");

  // Save canvas
  c->SaveAs(outputImage);
  std::cout << "Saved canvas to: " << outputImage << std::endl;
}
