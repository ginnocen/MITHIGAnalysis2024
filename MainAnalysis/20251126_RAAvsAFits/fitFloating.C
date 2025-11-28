// Minimal legend adjustments + "Charged hadron R_{AA}" everywhere,
// note about uncertainties, and BOLD pT range in the legend.

const int nPt = 15;
double pLow[nPt] = {3.2, 4.0, 4.8, 5.6, 6.4, 7.2, 9.6, 12.0, 14.4, 19.2, 24.0, 28.8, 35.2, 48.0, 73.6};
double pHigh[nPt] = {4.0, 4.8, 5.6, 6.4, 7.2, 9.6, 12.0, 14.4, 19.2, 24.0, 28.8, 35.2, 48.0, 73.6, 103.6};

// const int nPt = 13;
// double pLow[nPt] = {4.8, 5.6, 6.4, 7.2, 9.6, 12.0, 14.4, 19.2, 24.0, 28.8, 35.2, 48.0, 73.6};
// double pHigh[nPt] = {5.6, 6.4, 7.2, 9.6, 12.0, 14.4, 19.2, 24.0, 28.8, 35.2, 48.0, 73.6, 103.6};

TF1 *fit(int ptbin = 3, TString filename = "ptBinned_RAAVsA_Pow3-3_Version1.root") {

  // Input
  TFile *fileinput = TFile::Open(filename);
  if (!fileinput || fileinput->IsZombie()) {
    ::Error("fit", "Cannot open %s", filename.Data());
    return nullptr;
  }
  TGraphErrors *g = (TGraphErrors *)fileinput->Get(Form("gRAA_PtVsA_TotUncert_ptBin%d", ptbin));
  if (!g) {
    ::Error("fit", "Graph not found for ptbin %d", ptbin);
    return nullptr;
  }

  double xrangemindraw = 0.0;
  double xrangemaxdraw = 0.0;
  double xrangeminfit = 0.0;
  double xrangemaxfit = 0.0;

  xrangemindraw = 1.0;
  xrangemaxdraw = 220.0;
  xrangeminfit = 1.0;
  xrangemaxfit = 210.0;

  TVirtualFitter::SetDefaultFitter("Minuit2"); // optional but helpful

  TF1 *fitfunc = new TF1("fitfunc", "1 + [0]*(pow(x,[1]) - 1)", xrangeminfit, xrangemaxfit);
  fitfunc->SetParNames("m", "alpha");
  fitfunc->SetParameters(-0.15, 1.0 / 3.0);   // <-- crucial: nonzero seeds
  fitfunc->SetParLimits(0, -5.0, -1e-6);      // optional: keep m < 0, avoid m=0 plateau
  fitfunc->SetParLimits(1, 0.0, 1.0);         // optional: alpha >= 0
  TFitResultPtr r = g->Fit(fitfunc, "S R W"); // W uses your GraphErrors uncertainties

  // Axes (note the Y title)
  TH2F *hempty = new TH2F("hempty", "; A; R_{AA}", 100, xrangemindraw, xrangemaxdraw, 100, 0.0, 1.2);
  hempty->SetStats(0);

  // Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 900, 800);
  c1->SetMargin(0.12, 0.04, 0.12, 0.05);
  gPad->SetTicks(1, 1);
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
  g->SetFillColorAlpha(kBlue - 9, 0.35);
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
  l1->SetLineStyle(3); // dotted
  l1->SetLineColor(kGray + 2);
  l1->Draw("same");

  // Fit curve
  fitfunc->Draw("same");

  // Legend
  TLegend *leg = new TLegend(0.1380846, 0.22, 0.4376392, 0.409677);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);

  // Make ONLY the pT range entry bold
  TLegendEntry *eData = leg->AddEntry(g, Form("CMS %.1f < p_{T} < %.1f GeV/c", pLow[ptbin], pHigh[ptbin]), "p");
  if (eData)
    eData->SetTextFont(62); // 62 = Helvetica-Bold

  // Fit label (uses "Charged hadron R_{AA}")
  double m = fitfunc->GetParameter(0);
  double dm = fitfunc->GetParError(0);
  double alpha = fitfunc->GetParameter(1);
  double dalpha = fitfunc->GetParError(1);
  double chi2 = fitfunc->GetChisquare();
  int ndf = fitfunc->GetNDF();
  leg->AddEntry(fitfunc, Form("Fit: 1 %.2f * (A^{%.2f} - 1)", m, alpha), "");
  leg->AddEntry((TObject *)0, Form("#chi^{2}/ndf = %.4f", chi2 / ndf), "");
  leg->Draw();

  // Note on uncertainties
  TLatex *unc = new TLatex(0.15, 0.83, "Data uncertainties include stat., syst., and normalization");
  unc->SetNDC();
  unc->SetTextSize(0.04);
  unc->SetTextFont(42);
  unc->SetTextColor(kGray + 3);
  unc->Draw();

  // Output
  c1->SaveAs(Form("fit_ptBin%d_AdepFloating.png", ptbin));
  return fitfunc;
}

// Loop over all pT bins and fill an histogram with the m values extracted from the fit

void fitAFloating() {
  TF1 *fits[nPt];
  TH1F *hm = new TH1F("hm", "1 + m*(A^{#alpha} - 1); p_{T} (GeV); m", nPt, 0, nPt);
  TH1F *halpha = new TH1F("halpha", "1+ m*(A^{#alpha} - 1); p_{T} (GeV); #alpha", nPt, 0, nPt);
  hm->SetStats(0);
  halpha->SetStats(0);
  // define the x axis labels using pt intervals
  hm->GetXaxis()->SetNdivisions(nPt, kFALSE);
  halpha->GetXaxis()->SetNdivisions(nPt, kFALSE);
  hm->GetXaxis()->SetLabelSize(0.05);
  halpha->GetXaxis()->SetLabelSize(0.05);
  hm->GetYaxis()->SetLabelSize(0.05);
  halpha->GetYaxis()->SetLabelSize(0.05);
  hm->GetYaxis()->SetTitleSize(0.05);
  halpha->GetYaxis()->SetTitleSize(0.05);
  hm->GetYaxis()->SetTitleOffset(1.);
  halpha->GetYaxis()->SetTitleOffset(1.);
  hm->GetXaxis()->SetTitleOffset(1.2);
  halpha->GetXaxis()->SetTitleOffset(1.2);

  for (int i = 0; i < nPt; i++) {
    hm->GetXaxis()->SetBinLabel(i + 1, Form("%.1f-%.1f", pLow[i], pHigh[i]));
    halpha->GetXaxis()->SetBinLabel(i + 1, Form("%.1f-%.1f", pLow[i], pHigh[i]));
  }
  for (int ptbin = 0; ptbin < nPt; ptbin++) {
    fits[ptbin] = fit(ptbin);
    hm->SetBinContent(ptbin + 1, fits[ptbin]->GetParameter(0));
    hm->SetBinError(ptbin + 1, fits[ptbin]->GetParError(0));
    halpha->SetBinContent(ptbin + 1, fits[ptbin]->GetParameter(1));
    halpha->SetBinError(ptbin + 1, fits[ptbin]->GetParError(1));
  }
  TCanvas *c2 = new TCanvas("c2", "c2", 1600, 1200);
  c2->Divide(1, 2);
  c2->cd(1);
  hm->GetYaxis()->SetRangeUser(-1.0, 0.2);
  hm->Draw();
  c2->cd(2);
  halpha->GetYaxis()->SetRangeUser(0.0, 0.9);
  // draw a line at 1/3, 2/3
  halpha->Draw();
  TLine *l2 = new TLine(0.5, 2.0 / 3.0, nPt - 0.5, 2.0 / 3.0);
  TLine *l1 = new TLine(0.5, 1.0 / 3.0, nPt - 0.5, 1.0 / 3.0);
  l1->SetLineStyle(2); // dashed
  l1->SetLineColor(kBlue + 2);
  l1->SetLineWidth(2);
  l1->Draw("same");
  l2->SetLineStyle(3); // dashed
  l2->SetLineColor(kRed + 2);
  l2->SetLineWidth(3);
  l2->Draw("same");
  TLegend *leg2 = new TLegend(0.15, 0.75, 0.4, 0.9);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.06);
  leg2->SetTextFont(42);
  leg2->AddEntry(l1, "#alpha=1/3", "l");
  leg2->AddEntry(l2, "#alpha=2/3", "l");
  leg2->Draw();
  c2->SaveAs("m_vs_pTbin.png");
}
