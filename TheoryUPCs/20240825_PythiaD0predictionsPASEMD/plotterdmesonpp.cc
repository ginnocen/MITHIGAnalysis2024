#include "parampp.h"
void plotterdmesonpp(TString filename = "output.root",
                     TString outname = "crosssection.root") {
  gStyle->SetOptStat(0);
  TFile *fin = new TFile(filename.Data(), "READ");
  TH1F *hlumicounter = (TH1F *)fin->Get("hlumicounter");
  TH2F *hUnnormCrossSection = (TH2F *)fin->Get("hUnnormCrossSection");
  hUnnormCrossSection->SetName("hCrossSection");
  TH2F *hCrossSectionOverdptdy = (TH2F *)fin->Get("hUnnormCrossSection");
  hCrossSectionOverdptdy->SetName("hCrossSectionOverdptdy");
  hCrossSectionOverdptdy->SetTitle("D^{0}-only d#sigma/dp_{T}dy from Pythia8");
  hCrossSectionOverdptdy->GetYaxis()->SetTitle("D^{0} rapidity");
  hCrossSectionOverdptdy->GetXaxis()->SetTitle("D^{0} p_{T} [GeV/c]");

  for (int iptbin = 1; iptbin <= hCrossSectionOverdptdy->GetNbinsX();
       iptbin++) {
    for (int iybin = 1; iybin <= hCrossSectionOverdptdy->GetNbinsY(); iybin++) {
      double unnormcross = hUnnormCrossSection->GetBinContent(iptbin, iybin);
      double lumiinvnb = hlumicounter->GetBinContent(2) / 1e9;
      double binwidth = hUnnormCrossSection->GetXaxis()->GetBinWidth(iptbin) *
                        hUnnormCrossSection->GetYaxis()->GetBinWidth(iybin);
      double cross = unnormcross / (2. * lumiinvnb * binwidth);
      hCrossSectionOverdptdy->SetBinContent(iptbin, iybin, cross);
      std::cout << "interval pt"
                << hUnnormCrossSection->GetXaxis()->GetBinLowEdge(iptbin) << " "
                << hUnnormCrossSection->GetXaxis()->GetBinUpEdge(iptbin);
      std::cout << " , interval y"
                << hUnnormCrossSection->GetYaxis()->GetBinLowEdge(iybin) << " "
                << hUnnormCrossSection->GetYaxis()->GetBinUpEdge(iybin);
      std::cout << " , cross section for D0-only in pb: " << cross << std::endl;
    }
  }
  int nybin = hCrossSectionOverdptdy->GetNbinsY();
  if (nybin != 1)
    std::cout << "More than one rapidity bin, so we can't project "
                 "the cross section over p_{T} and plot it."
              << std::endl;
  TH1D *hCrossSectionOverdptYint =
      hCrossSectionOverdptdy->ProjectionX("hCrossSectionOverdptYint");
  hCrossSectionOverdptYint->SetName("hCrossSectionOverdptYint");
  std::cout << "Y-bin interval width is "
            << hCrossSectionOverdptdy->GetYaxis()->GetBinWidth(1) << std::endl;
  hCrossSectionOverdptYint->Scale(
      hCrossSectionOverdptdy->GetYaxis()->GetBinWidth(1));
  TCanvas *c1 = new TCanvas("c1", "c1", 900, 800);
  gPad->SetLogy();
  gPad->SetLogx();
  hCrossSectionOverdptYint->SetMaximum(hCrossSectionOverdptYint->GetMaximum() * 100);
  hCrossSectionOverdptYint->SetMinimum(0.1);
  hCrossSectionOverdptYint->SetMarkerStyle(20);
  hCrossSectionOverdptYint->SetMarkerSize(0.8);
  hCrossSectionOverdptYint->SetMarkerColor(kBlue);
  hCrossSectionOverdptYint->SetLineColor(kBlue);
  hCrossSectionOverdptYint->Draw("HIST");
  hCrossSectionOverdptYint->GetYaxis()->SetTitle(
      "D^{0} d#sigma/dp_{T} for |y|<1.0  [pb/GeV]");
  hCrossSectionOverdptYint->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hCrossSectionOverdptYint->SetTitle("");
  hCrossSectionOverdptYint->GetYaxis()->SetTitleOffset(1.2);
  hCrossSectionOverdptYint->GetXaxis()->SetTitleOffset(1.3);
  TFile *gFONLLCMS = new TFile("CMSdataD0pp_1708.04962/CrossSectionFONLLPPMB.root", "READ");
  TGraphAsymmErrors *gaeSigmaDzero =
      (TGraphAsymmErrors *)gFONLLCMS->Get("gaeSigmaDzero");
  gaeSigmaDzero->SetName("gaeSigmaDzero");
  gaeSigmaDzero->SetLineColor(kRed);
  gaeSigmaDzero->Draw("Psame");
  TFile *gFONLLCMSHighPt = new TFile("CMSdataD0pp_1708.04962/CrossSectionFONLLPP.root", "READ");
  TGraphAsymmErrors *gaeSigmaDzeroHighPt =
      (TGraphAsymmErrors *)gFONLLCMSHighPt->Get("gaeSigmaDzero");
  gaeSigmaDzeroHighPt->SetName("gaeSigmaDzeroHighPt");
  gaeSigmaDzeroHighPt->SetLineColor(kRed);
  gaeSigmaDzeroHighPt->Draw("Psame");
  TFile *fCMSdata = new TFile("CMSdataD0pp_1708.04962/HEPData-ins1616207-v1-Table_2.root", "READ");
  TH1D *gCMSdata = (TH1D *)fCMSdata->Get("Table 2/Graph1D_y1");
  gCMSdata->SetName("gCMSdata");
  gCMSdata->SetMarkerStyle(20);
  gCMSdata->SetMarkerSize(0.8);
  gCMSdata->SetMarkerColor(kBlack);
  gCMSdata->SetLineColor(kBlack);
  gCMSdata->Draw("PEsame");
  hCrossSectionOverdptYint->Draw("same");
  TLegend *leg = new TLegend(0.15, 0.75, 0.75, 0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.03);
  leg->AddEntry(hCrossSectionOverdptYint, "Pythia8 prompt D^{0} pp Monash", "lp");
  leg->AddEntry(gaeSigmaDzero, "FONLL prompt D^{0} pp BR=0.577", "lp");
  leg->AddEntry(gCMSdata, "CMS prompt D^{0} pp 5.02 TeV, PLB 782 (2018) 474", "lp");
  leg->Draw();
  c1->SaveAs("hCrossSectionOverdptYint.png");
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  hCrossSectionOverdptYint->Write();
  fout->Close();

  TCanvas *ratioPythiaOverFONLL = new TCanvas("ratioPythiaOverFONLL", "ratioPythiaOverFONLL", 900, 800);
  gPad->SetLogx();
  TH1F *hRatioPythiaOverFONLL = (TH1F *)hCrossSectionOverdptYint->Clone("hRatioPythiaOverFONLL");
  for (int i = 1; i <= hCrossSectionOverdptYint->GetNbinsX(); i++) {
    double x = hCrossSectionOverdptYint->GetBinCenter(i);
    double y = hCrossSectionOverdptYint->GetBinContent(i);
    double y_fonll;
    if (i<=9) y_fonll = gaeSigmaDzero->Eval(x);
    else y_fonll = gaeSigmaDzeroHighPt->Eval(x);
    double ratio = y / y_fonll;
    hRatioPythiaOverFONLL->SetBinContent(i, ratio); 
    hRatioPythiaOverFONLL->SetBinError(i, ratio*hCrossSectionOverdptYint->GetBinError(i)/y);
  }
  hRatioPythiaOverFONLL->GetYaxis()->SetTitle("Pythia8 Monash/FONLL");
  hRatioPythiaOverFONLL->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hRatioPythiaOverFONLL->SetMinimum(0.0);
  hRatioPythiaOverFONLL->SetMaximum(2.0);
  hRatioPythiaOverFONLL->SetMarkerStyle(20);
  hRatioPythiaOverFONLL->SetMarkerSize(0.8);
  hRatioPythiaOverFONLL->SetMarkerColor(kBlue);
  hRatioPythiaOverFONLL->SetLineColor(kBlue);
  hRatioPythiaOverFONLL->Draw("PE");
  ratioPythiaOverFONLL->SaveAs("hRatioPythiaOverFONLL.png");

  TFile *fref = new TFile("crosssection_ref_pp.root", "READ");
  TH1D *hCrossSectionOverdptYint_ref =
      (TH1D *)fref->Get("hCrossSectionOverdptYint");
  hCrossSectionOverdptYint_ref->SetName("hCrossSectionOverdptYint_ref");
  for (int i = 1; i <= hCrossSectionOverdptYint->GetNbinsX(); i++) {
    double x = hCrossSectionOverdptYint->GetBinCenter(i);
    double y = hCrossSectionOverdptYint->GetBinContent(i);
    double y_ref = hCrossSectionOverdptYint_ref->GetBinContent(i);
    double ratio = y / y_ref;
    std::cout << "p_{T} = " << x << " GeV/c, cross section = " << y
              << " pb, ratio = " << ratio << std::endl;
  }
}
