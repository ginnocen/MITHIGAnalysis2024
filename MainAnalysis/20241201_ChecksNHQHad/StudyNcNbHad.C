void StudyNcNbHad(TString myvar = "Nc") {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TFile f("/data/NewSkims_gtoccbar/"
          "SkimMC2018PbPb_Version20241201_InputForest_20241201_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_"
          "PARTIAL.root");
  TTree *Tree = (TTree *)f.Get("Tree");
  const int kCases = 3;
  TH1F *hNHFHad[kCases];
  TString cuts[kCases] = {"JetPT > 100.",
                          "mumuMass > 0. && fabs(muDiDxy1) > 0.01 && fabs(muDiDxy2) > 0.01 && JetPT > 100.",
                          "mumuMass > 0. && fabs(muDiDxy1) > 0.01 && fabs(muDiDxy2) > 0.01 && fabs(muDiDxy1) < 0.05 && "
                          "fabs(muDiDxy2) < 0.05 && JetPT > 100."};
  TString titles[kCases] = {"JetPT > 100 GeV", "+ MuMu Tagged, |DCAXY| muons > 0.01 cm", "+ |DCAXY| < 0.05 cm"};
  int colors[kCases] = {kBlack, kRed, kBlue};
  int markers[kCases] = {20, 21, 22};

  for (int i = 0; i < kCases; i++) {
    hNHFHad[i] = new TH1F(Form("hNHFHad_%d", i), Form(";N_{HF}^{had}"), 10, -0.5, 9.5);
    hNHFHad[i]->Sumw2();
    Tree->Draw(Form("MJT%sHad>>hNHFHad_%d", myvar.Data(), i), cuts[i].Data());
    // hNHFHad[i]->Scale(1./hNHFHad[i]->Integral());
  }
  TCanvas *cNHFHad = new TCanvas("cNHFHad", "cNHFHad", 1000, 1000);
  gPad->SetLogy();
  cNHFHad->cd();
  TLegend *leg = new TLegend(0.4, 0.75, 0.7, 0.9);
  leg->SetHeader(Form("N_{HF}^{had} (%s)", myvar.Data()));
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.025);
  for (int i = 0; i < kCases; i++) {
    hNHFHad[i]->SetLineColor(colors[i]);
    hNHFHad[i]->SetMarkerColor(colors[i]);
    hNHFHad[i]->SetMarkerStyle(markers[i]);
    hNHFHad[i]->SetMarkerSize(1.5);
    hNHFHad[i]->SetMaximum(hNHFHad[i]->GetMaximum() * 10.);
    hNHFHad[i]->SetMinimum(0.001);
    hNHFHad[i]->GetXaxis()->SetRangeUser(0., 5.5);
    if (i == 0)
      hNHFHad[i]->Draw();
    else
      hNHFHad[i]->Draw("same");
    leg->AddEntry(hNHFHad[i], titles[i], "lp");
    std::cout << "Entries for " << titles[i] << " = " << hNHFHad[i]->GetEntries() << std::endl;
  }
  leg->Draw();
  cNHFHad->SaveAs(Form("Study%sHad.png", myvar.Data()));

  TCanvas *cNbNcHad = new TCanvas("cNbNcHad", "cNbNcHad", 1000, 1000);
  cNbNcHad->cd();
  TH2D *hNbNcHad = new TH2D("hNbNcHad", ";N_{c}^{had};N_{b}^{had}", 6, -0.5, 5.5, 6, -0.5, 5.5);
  hNbNcHad->Sumw2();
  Tree->Draw(Form("MJTNbHad:MJTNcHad>>hNbNcHad"), cuts[2].Data());
  hNbNcHad->Draw("colz text");
  cNbNcHad->SaveAs(Form("Study%sNbNcHad.png", myvar.Data()));

  TFile fOut(Form("Study%sHad.root", myvar.Data()), "recreate");
  for (int i = 0; i < kCases; i++) {
    hNHFHad[i]->Write();
  }
  fOut.Close();
}
