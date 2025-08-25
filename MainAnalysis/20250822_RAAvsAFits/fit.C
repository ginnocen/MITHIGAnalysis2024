// Minimal legend adjustments + "Charged hadron R_{AA}" everywhere,
// note about uncertainties, and BOLD pT range in the legend.

const int nPt = 5;
double pLow[nPt]  = {9.6, 14.4, 24.0, 35.2, 48.0};
double pHigh[nPt] = {12.0, 19.2, 28.8, 48.0, 73.6};

void fit(int ptbin = 3, TString filename = "extractedGraphs.root", int option = 1) {

  // Input
  TFile *fileinput = TFile::Open(filename);
  if (!fileinput || fileinput->IsZombie()) { ::Error("fit","Cannot open %s", filename.Data()); return; }
  TGraphErrors *g = (TGraphErrors *)fileinput->Get(Form("gRAA_PtVsA_Tot_ptBin%d_option%d", ptbin, option));
  if (!g) { ::Error("fit","Graph not found for ptbin=%d option=%d", ptbin, option); return; }

  // Fit
  TF1 *fitfunc = new TF1("fitfunc", "1 + [0]*(x-1)", 0.5, 10);
  fitfunc->SetLineColor(kRed+1);
  fitfunc->SetLineWidth(6);
  fitfunc->SetLineStyle(2); // dashed
  TFitResultPtr r = g->Fit(fitfunc, "S R0");

  // Axes (note the Y title)
  TH2D *hempty = new TH2D("hempty", "; A^{1/3}; Charged hadron R_{AA}", 10, 0.5, 8, 10, 0.0, 1.25);
  hempty->SetStats(0);

  // Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 900, 800);
  c1->SetMargin(0.12, 0.04, 0.12, 0.05);
  gPad->SetTicks(1,1);
  hempty->GetXaxis()->SetTitleOffset(1.1);
  hempty->GetYaxis()->SetTitleOffset(1.1);
  hempty->GetYaxis()->SetTitleSize(0.045);
  hempty->GetYaxis()->SetLabelSize(0.035);
  hempty->GetXaxis()->SetTitleSize(0.045);
  hempty->GetXaxis()->SetLabelSize(0.035);
  hempty->GetXaxis()->SetNdivisions(505);
  hempty->GetYaxis()->SetNdivisions(505);
  hempty->Draw();

  // Graph style
  g->SetFillColorAlpha(kBlue-9, 0.35);
  g->SetFillStyle(1001);
  g->SetLineColor(kBlack);
  g->SetLineWidth(1);
  g->SetMarkerStyle(20);
  g->SetMarkerColor(kBlack);
  g->SetMarkerSize(1.3);

  // Draw order
  g->Draw("E2 SAME");
  g->Draw("E1 SAME");
  g->Draw("P  SAME");

  // Unity line (dotted)
  TLine *l1 = new TLine(0.5, 1.0, 8.0, 1.0);
  l1->SetLineStyle(3);          // dotted
  l1->SetLineColor(kGray+2);
  l1->Draw("same");

  // Fit curve
  fitfunc->Draw("same");

  // Legend
  TLegend *leg = new TLegend(0.1380846,0.1496774,0.4376392,0.3509677);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);

  // Make ONLY the pT range entry bold
  TLegendEntry* eData =
      leg->AddEntry(g, Form("CMS %.1f < p_{T} < %.1f GeV/c", pLow[ptbin], pHigh[ptbin]), "p");
  if (eData) eData->SetTextFont(62);  // 62 = Helvetica-Bold

  //leg->AddEntry(fitfunc, Form("Fit: 1 + (%.3f #pm %.3f)(A^{1/3} - 1)",
  //	 fitfunc->GetParameter(0), fitfunc->GetParError(0)), "l");

  // Fit label (uses "Charged hadron R_{AA}")
  double m   = fitfunc->GetParameter(0);
  double dm  = fitfunc->GetParError(0);
  double chi2 = fitfunc->GetChisquare();
  int    ndf  = fitfunc->GetNDF();
  leg->AddEntry(fitfunc, Form("Fit: 1 + m (A^{1/3} - 1), m = %.3f #pm %.3f", m, dm), "l"); 
  leg->Draw();

  TLatex *latex = new TLatex(0.5, 0.6,
    Form("#chi^{2}/NDF = %.2f #pm %.2f", chi2/ndf, TMath::Sqrt(chi2)/ndf));
  latex->SetNDC();
  latex->SetTextSize(0.045);
  latex->SetTextFont(42);
  latex->Draw();

  // Note on uncertainties
  TLatex *unc = new TLatex(0.15, 0.83,
    "Data uncertainties include stat., syst., and normalization");
  unc->SetNDC();
  unc->SetTextSize(0.04);
  unc->SetTextFont(42);
  unc->SetTextColor(kGray+3);
  unc->Draw();

  // Output
  c1->SaveAs(Form("fit_ptBin%d_option%d.png", ptbin, option));
}
