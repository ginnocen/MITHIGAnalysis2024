void draw(TString string1 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_1",
          TString string2 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_1", TString histname = "hNtrk", float xmin = 0,
          float xmax = 100, float ymin = 0, float ymax = 100, bool logy = true, bool logx = false) {

  TFile *f1 = TFile::Open(Form("output_loop_%s.root", string1.Data()));
  TFile *f2 = TFile::Open(Form("output_loop_%s.root", string2.Data()));

  TH1F *h1 = (TH1F *)f1->Get(histname);
  TH1F *h2 = (TH1F *)f2->Get(histname);

  TCanvas *c1 = new TCanvas("c1", "Comparison", 800, 600);
  gStyle->SetOptStat(0);
  if (logy)
    c1->SetLogy();
  if (logx)
    c1->SetLogx();
  h1->GetXaxis()->SetRangeUser(xmin, xmax);
  h1->GetYaxis()->SetRangeUser(ymin, ymax);
  h1->Scale(1.0 / h1->Integral());
  h2->Scale(1.0 / h2->Integral());
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h1->Draw();
  h2->Draw("SAME");
  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(h1, string1, "l");
  legend->AddEntry(h2, string2, "l");
  legend->Draw();
  c1->SaveAs(Form("comparison_%s_vs_%s_%s.png", string1.Data(), string2.Data(), histname.Data()));
}

void compare(TString string1 = "ppsTag_0_zdcPlusVeto_0_HFPlusGapVeto_0",
             TString string2 = "ppsTag_0_zdcPlusVeto_1_HFPlusGapVeto_1") {
  draw(string1, string2, "hNtr", 0, 100, 1e-4, 10, true, false);
  draw(string1, string2, "hEtaCharged", -6, 6, 1e-4, 0.5, true, false);
}
