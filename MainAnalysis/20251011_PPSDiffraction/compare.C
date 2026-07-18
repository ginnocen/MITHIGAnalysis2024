// Compare histograms from two different output files
void draw(TString string1 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_1",
          TString string2 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_1",
          TString histname = "hNtrk",
          float xmin = 0, float xmax = 100,
          float ymin = 1e-6, float ymax = 100,  // ymin > 0 for logY
          bool logy = true, bool logx = false) {

  TFile *f1 = TFile::Open(Form("output_loop_%s.root", string1.Data()));
  TFile *f2 = TFile::Open(Form("output_loop_%s.root", string2.Data()));
  TH1F *h1 = (TH1F*)f1->Get(histname);
  TH1F *h2 = (TH1F*)f2->Get(histname);

  TCanvas *c1 = new TCanvas("c1", "Comparison", 800, 700);
  gStyle->SetOptStat(0);
  if (logy) c1->SetLogy();
  if (logx) c1->SetLogx();

  // Normalize safely
  double i1 = h1->Integral();
  double i2 = h2->Integral();
  if (i1 > 0) h1->Scale(1.0/i1);
  if (i2 > 0) h2->Scale(1.0/i2);

  // X range zoom is fine with SetRangeUser on X
  h1->GetXaxis()->SetRangeUser(xmin, xmax);

  // Y range must use SetMinimum/SetMaximum on the FIRST drawn hist
  if (logy && ymin <= 0) ymin = 1e-6; // enforce >0 on log pad
  h1->SetMinimum(ymin);
  h1->SetMaximum(ymax);

  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);

  h1->Draw("HIST");         // first draw defines the frame
  h2->Draw("HIST SAME");    // no autoscale now

  TLegend *legend = new TLegend(0.2, 0.75, 0.9, 0.9);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->AddEntry(h1, string1, "l");
  legend->AddEntry(h2, string2, "l");
  legend->Draw();

  TLatex t; t.SetNDC(); t.SetTextSize(0.03); t.SetTextFont(42);
  t.DrawLatex(0.15, 0.93, "CMS p-O #sqrt{s_{NN}} = 9.62 TeV, 2025");

  c1->Modified(); c1->Update();
  c1->SaveAs(Form("plots/comparison_%s_vs_%s_%s.png",
                  string1.Data(), string2.Data(), histname.Data()));
}
void compare(TString string1 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_0",
             TString string2 = "ppsTag_0_zdcPlusVeto_1_HFPlusGapVeto_1") {
  draw(string1, string2, "hNtr", 0, 100, 1e-4, 10, true, false);
  draw(string1, string2, "hEtaCharged", -3, 3, 1e-4, 0.5, true, false);
  draw(string1, string2, "hZDCm", 0, 6000, 1e-5, 2, true, false);
  draw(string1, string2, "hZDCp", 0, 6000, 1e-5, 2, true, false);
}
