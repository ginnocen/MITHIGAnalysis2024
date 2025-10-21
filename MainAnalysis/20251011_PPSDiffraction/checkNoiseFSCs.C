void draw(TString filename = "20250703_pO_PhysicsIonPhysics0_393952_full.root",
          TString varname = "FSC2bottomM_chargefC", TString label = "FSC2 Bottom Charge (fC)",
          TString selection = "passBaselineEventSelection&&HLT_OxyZeroBias_v1") {

  TFile *f = TFile::Open(filename);
  const int nTimeSlices = 5;
  TH1F *histograms[nTimeSlices];
  // create empty histograms
  for (int i = 0; i < nTimeSlices; i++) {
    histograms[i] = new TH1F(Form("%s_timeSlice%d", varname.Data(), i), Form("%s_timeSlice%d", varname.Data(), i),
                             10000, 0, 500000);
    histograms[i]->Sumw2();
    histograms[i]->GetXaxis()->SetTitle(label);
    histograms[i]->GetXaxis()->SetTitleOffset(1.2);
    histograms[i]->GetYaxis()->SetTitle("Counts");
    histograms[i]->SetTitle("");
  }
  TTree *Tree = (TTree *)f->Get("Tree");
  for (int i = 0; i < nTimeSlices; i++) {
    Tree->Draw(Form("%s[%d]>>%s_timeSlice%d", varname.Data(), i, varname.Data(), i), selection.Data(), "goff");
    histograms[i]->Scale(1.0 / histograms[i]->GetEntries());
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
  c1->cd();
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  gPad->SetLogx();
  // define colors for different time slices
  int colors[nTimeSlices] = {kRed, kBlue, kGreen + 2, kBlack, kMagenta};
  // overlap histograms from different time slices
  TLegend *legend = new TLegend(0.1472946, 0.6645161, 0.3466934, 0.8632258);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.03);
  legend->SetFillStyle(0);
  // set title of legend according to label
  legend->SetHeader(label);

  for (int i = 0; i < nTimeSlices; i++) {
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->SetLineWidth(2);
    histograms[i]->SetMaximum(1.0);
    histograms[i]->SetMinimum(1e-6);
    histograms[i]->GetXaxis()->SetRangeUser(50, 500000);
    if (i == 0) {
      histograms[i]->Draw();
    } else {
      histograms[i]->Draw("SAME");
    }
    legend->AddEntry(histograms[i], Form("Time Slice %d", i), "l");
  }
  legend->Draw();
  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.03);
  latex->SetTextFont(42);
  latex->DrawLatex(0.15, 0.93, "CMS p-O #sqrt{s_{NN}} = 9.62 TeV, 2025");
  latex->DrawLatex(0.6, 0.93, selection.ReplaceAll("passBaselineEventSelection&&", "").ReplaceAll("_v1", ""));
  c1->SaveAs(Form("plots/checkNoiseFSCs_%s.png", varname.Data()));
}

void checkNoiseFSCs() {
  draw("20250703_pO_PhysicsIonPhysics0_393952_full.root", "FSC2bottomM_chargefC", "FSC2 Bottom Charge (fC)");
  draw("20250703_pO_PhysicsIonPhysics0_393952_full.root", "FSC3bottomleftM_chargefC", "FSC3 Bottom Left Charge (fC)");
}
