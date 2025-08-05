void referenceplots(TString fileName = "tempOO_Reference") {
  TFile *file = TFile::Open(Form("%s.root", fileName.Data()), "READ");
  TTree *tree = (TTree *)file->Get("Tree");

  TH1F *hPtNominal = new TH1F("hPtNominal", "hPtNominal", 20, 0, 20);
  TH1F *hPtLoose = new TH1F("hPtLoose", "hPtLoose", 20, 0, 20);
  TH1F *hPtTight = new TH1F("hPtTight", "hPtTight", 20, 0, 20);
  TH1F *hmultiplicityEta2p4 = new TH1F("hmultiplicityEta2p4", "hmultiplicityEta2p4", 300, 0, 300);

  tree->Draw("trkPt>>hPtNominal",
             "passBaselineEventSelection&&trkPassChargedHadron_Nominal*trackingEfficiency_Nominal");
  tree->Draw("trkPt>>hPtLoose", "passBaselineEventSelection&&trkPassChargedHadron_Loose*trackingEfficiency_Loose");
  tree->Draw("trkPt>>hPtTight", "passBaselineEventSelection&&trkPassChargedHadron_Tight*trackingEfficiency_Tight");
  tree->Draw("multiplicityEta2p4>>hmultiplicityEta2p4", "passBaselineEventSelection");
  // tree->Draw("trkPt>>hPtNominal", "trkPassChargedHadron_Nominal");
  // tree->Draw("trkPt>>hPtLoose", "trkPassChargedHadron_Loose");
  // tree->Draw("trkPt>>hPtTight", "trkPassChargedHadron_Tight");

  TCanvas *c1 = new TCanvas("c1", "c1", 2200, 600);
  c1->Divide(3, 1);
  c1->cd(1);
  gPad->SetLogy();
  hPtNominal->SetMinimum(1);
  hPtNominal->SetMaximum(1e7);
  hPtNominal->SetLineColor(kBlack);
  hPtLoose->SetLineColor(kBlue);
  hPtTight->SetLineColor(kRed);
  hPtNominal->Draw();
  hPtLoose->Draw("same");
  hPtTight->Draw("same");
  TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
  legend->AddEntry(hPtNominal, "Nominal", "l");
  legend->AddEntry(hPtLoose, "Loose", "l");
  legend->AddEntry(hPtTight, "Tight", "l");
  legend->Draw();
  c1->cd(2);
  // Draw the ratio of the histograms with respect to the nominal
  TH1F *hPtLooseRatio = (TH1F *)hPtLoose->Clone("hPtLooseRatio");
  hPtLooseRatio->Divide(hPtNominal);
  TH1F *hPtTightRatio = (TH1F *)hPtTight->Clone("hPtTightRatio");
  hPtTightRatio->Divide(hPtNominal);
  hPtLooseRatio->SetLineColor(kBlue);
  hPtTightRatio->SetLineColor(kRed);
  hPtLooseRatio->SetMinimum(0.5);
  hPtLooseRatio->SetMaximum(1.5);
  hPtLooseRatio->Draw();
  hPtTightRatio->Draw("same");
  TLegend *ratioLegend = new TLegend(0.6, 0.7, 0.9, 0.9);
  ratioLegend->AddEntry(hPtLooseRatio, "Loose / Nominal", "l");
  ratioLegend->AddEntry(hPtTightRatio, "Tight / Nominal", "l");
  ratioLegend->Draw();
  c1->cd(3);
  gPad->SetLogy();
  hmultiplicityEta2p4->SetLineColor(kBlack);
  hmultiplicityEta2p4->SetMinimum(1);
  hmultiplicityEta2p4->SetMaximum(1e7);
  hmultiplicityEta2p4->Draw();
  c1->SaveAs(Form("trackingEfficiencyComparison_%s.pdf", fileName.Data()));
}
