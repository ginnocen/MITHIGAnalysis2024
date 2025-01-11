#include "param.h"
void studycoverage(TString filename = "output.root",
                      TString outname = "crosssection.root") {
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TFile *fin = new TFile(filename.Data(), "READ");
  TH1F *hlumicounter = (TH1F *)fin->Get("hlumicounter");
  TH2F *hUnnormCrossSection = (TH2F *)fin->Get("hUnnormCrossSection");
  hUnnormCrossSection->SetName("hCrossSection");
  TH2F *hCrossSection = (TH2F *)fin->Get("hUnnormCrossSection");
  hCrossSection->SetName("hCrossSection");
  hCrossSection->SetTitle("D^{0}-only d#sigma/dp_{T}dy from Pythia8");
  hCrossSection->GetYaxis()->SetTitle("D^{0} rapidity");
  hCrossSection->GetXaxis()->SetTitle("D^{0} p_{T} [GeV/c]");
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      double unnormcross = hUnnormCrossSection->GetBinContent(iptbin, iybin);
      double lumiinvnb = hlumicounter->GetBinContent(2) / 1e6;
      double binwidth = hUnnormCrossSection->GetXaxis()->GetBinWidth(iptbin) *
                        hUnnormCrossSection->GetYaxis()->GetBinWidth(iybin);
      double cross = unnormcross / (2. * lumiinvnb * binwidth);
      hCrossSection->SetBinContent(iptbin, iybin, cross);
      std::cout << "interval pt"
                << hUnnormCrossSection->GetXaxis()->GetBinLowEdge(iptbin) << " "
                << hUnnormCrossSection->GetXaxis()->GetBinUpEdge(iptbin);
      std::cout << " , interval y"
                << hUnnormCrossSection->GetYaxis()->GetBinLowEdge(iybin) << " "
                << hUnnormCrossSection->GetYaxis()->GetBinUpEdge(iybin);
      std::cout << " , cross section for D0-only in nb: " << cross << std::endl;
    }
  }
  std::vector<TH1D *> hcrossvsy;
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    TString hname = Form("hcrossvsy_%d", iptbin);
    TH1D *h =
        new TH1D(hname.Data(),
                 "; D^{0} rapidity; d#sigma/dyp_{T} for particle only in nb",
                 nYBins, yBins);
    h->SetTitle(
        Form("Pythia d#sigma/dp_{T}dy (nb) for D^{0}-only p_{T} [%.1f, %.1f]",
             ptBins[iptbin - 1], ptBins[iptbin]));
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      h->SetBinContent(iybin, hCrossSection->GetBinContent(iptbin, iybin));
    }
    hcrossvsy.push_back(h);
  }
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  for (int iptbin = 0; iptbin < hcrossvsy.size(); iptbin++) {
    hcrossvsy[iptbin]->SetName(Form("hDzerocrossbinnedvsy_pt%d", iptbin));
    hcrossvsy[iptbin]->Write();
  }
  hCrossSection->Write();
  fout->Close();
  TCanvas *c1 = new TCanvas("c1", "c1", 1400, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  hCrossSection->GetYaxis()->SetTitleOffset(1.3);
  hCrossSection->Draw("colz text");
  c1->cd(2);
  gPad->SetLogy();
  TLegend *leg = new TLegend(0.5, 0.75, 0.8, 0.9);
  hcrossvsy[0]->SetTitle("Pythia d#sigma/dy for D^{0}-only");
  hcrossvsy[0]->SetMinimum(1.);
  hcrossvsy[0]->SetMaximum(1e9);
  leg->AddEntry(hcrossvsy[0], Form("p_{T} [%.1f, %.1f]", ptBins[0], ptBins[1]),
                "l");
  hcrossvsy[0]->SetLineColor(kBlack);
  hcrossvsy[0]->Draw();

  for (int iptbin = 1; iptbin < hcrossvsy.size(); iptbin++) {
    hcrossvsy[iptbin]->SetLineColor(iptbin + 1);
    hcrossvsy[iptbin]->Draw("hist same");
    leg->AddEntry(
        hcrossvsy[iptbin],
        Form("p_{T} [%.1f, %.1f]", ptBins[iptbin], ptBins[iptbin + 1]), "l");
  }
  leg->Draw();
  c1->SaveAs("plotterdmesonUPC.pdf");
  c1->SaveAs("plotterdmesonUPC.png");
  TFile *fref = new TFile("crosssection_ref.root", "READ");
  TH2F *href = (TH2F *)fref->Get("hCrossSection");
  href->SetName("href");
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      double cross = hCrossSection->GetBinContent(iptbin, iybin);
      double refcross = href->GetBinContent(iptbin, iybin);
      if (refcross > 0) {
        if (cross / refcross != 1) {
          std::cout << "cross section ratio: " << cross / refcross << std::endl;
        } else {
          std::cout << "ratio is=" << cross / refcross << std::endl;
        }
      }
    }
  }
  TH1D *hphotonEnergy[nPtBins][nYBins];
  TH1D *hgluonxBjorken[nPtBins][nYBins];
  TH1D *hgluonQ2[nPtBins][nYBins];

  float rebin[nPtBins][nYBins] = {
      {10, 10, 10, 10}, {100, 100, 100, 100}, {1000, 1000, 1000, 1000}};
  float rebinQ2[nPtBins][nYBins] = {
      {2, 2, 2, 2}, {10, 10, 10, 10}, {10, 10, 10, 10}};
  float rebinEgamma[nPtBins][nYBins] = {
      {2, 2, 2, 2}, {5, 5, 5, 5}, {10, 10, 10, 10}};

  for (int iptbin = 0; iptbin < nPtBins; iptbin++) {
    for (int iybin = 0; iybin < nYBins; iybin++) {
      hphotonEnergy[iptbin][iybin] =
          (TH1D *)fin->Get(Form("hphotonEnergy_pt%d_y%d", iptbin, iybin));
      hphotonEnergy[iptbin][iybin]->SetName(
          Form("hphotonEnergy_pt%d_y%d", iptbin, iybin));
      hphotonEnergy[iptbin][iybin]->Rebin(rebinEgamma[iptbin][iybin]);
      hphotonEnergy[iptbin][iybin]->Scale(
          1. / hphotonEnergy[iptbin][iybin]->Integral(
                   1, hphotonEnergy[iptbin][iybin]->GetNbinsX()));
      std::cout << "hphotonEnergy[iptbin][iybin]->Integral()="
                << hphotonEnergy[iptbin][iybin]->Integral() << std::endl;
      hgluonxBjorken[iptbin][iybin] =
          (TH1D *)fin->Get(Form("hgluonxBjorken_pt%d_y%d", iptbin, iybin));
      hgluonxBjorken[iptbin][iybin]->SetName(
          Form("hgluonxBjorken_pt%d_y%d", iptbin, iybin));
      hgluonxBjorken[iptbin][iybin]->Rebin(rebin[iptbin][iybin]);
      hgluonxBjorken[iptbin][iybin]->Scale(
          1. / hgluonxBjorken[iptbin][iybin]->Integral(
                   1, hgluonxBjorken[iptbin][iybin]->GetNbinsX()));
      std::cout << "hgluonxBjorken[iptbin][iybin]->Integral()="
                << hgluonxBjorken[iptbin][iybin]->Integral() << std::endl;
      hgluonQ2[iptbin][iybin] =
          (TH1D *)fin->Get(Form("hgluonQ2_pt%d_y%d", iptbin, iybin));
      hgluonQ2[iptbin][iybin]->SetName(
          Form("hgluonQ2_pt%d_y%d", iptbin, iybin));
      hgluonQ2[iptbin][iybin]->Rebin(rebinQ2[iptbin][iybin]);
      hgluonQ2[iptbin][iybin]->Scale(
          1. / hgluonQ2[iptbin][iybin]->Integral(
                   1, hgluonQ2[iptbin][iybin]->GetNbinsX()));
      std::cout << "hgluonQ2[iptbin][iybin]->Integral()="
                << hgluonQ2[iptbin][iybin]->Integral() << std::endl;
    }
  }

  TCanvas *cPhotonproperties =
      new TCanvas("cPhotonproperties", "cPhotonproperties", 1800, 600);
  cPhotonproperties->Divide(3, 1);
  cPhotonproperties->cd(1);
  TLegend *legphoton[nPtBins];
  bool swithonplottingphoton[nPtBins][nYBins] = {{true, true, true, true},
                                                 {true, true, true, true},
                                                 {true, true, true, true}};
  float xlowphoton[nPtBins] = {0.5, 0.5, 0.5};
  float xupphoton[nPtBins] = {300., 300., 300.};
  float ymaxphoton[nPtBins] = {5., 5., 5.};
  float yminphoton[nPtBins] = {0.0001, 0.0001, 0.0001};
  TH2F *hemptyEgamma[nPtBins];
  for (int iptbin = 0; iptbin < nPtBins; iptbin++) {
    cPhotonproperties->cd(iptbin + 1);
    gPad->SetLogy();
    gPad->SetLogx();
    legphoton[iptbin] = new TLegend(0.3, 0.65, 0.8, 0.86);
    legphoton[iptbin]->SetTextSize(0.03);
    legphoton[iptbin]->SetTextFont(42);
    legphoton[iptbin]->SetBorderSize(0);
    hemptyEgamma[iptbin] =
        new TH2F("hemptyEgamma", "", 100, xlowphoton[iptbin], xupphoton[iptbin],
                 100, yminphoton[iptbin], ymaxphoton[iptbin]);
    hemptyEgamma[iptbin]->GetYaxis()->SetTitleOffset(1.3);
    hemptyEgamma[iptbin]->GetYaxis()->SetTitle(
        "Self normalized dN/dE_{#gamma}");
    hemptyEgamma[iptbin]->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    hemptyEgamma[iptbin]->GetXaxis()->SetRangeUser(xlowphoton[iptbin],
                                                   xupphoton[iptbin]);
    hemptyEgamma[iptbin]->SetMinimum(yminphoton[iptbin]);
    hemptyEgamma[iptbin]->SetMaximum(ymaxphoton[iptbin]);
    hemptyEgamma[iptbin]->Draw();
    for (int iybin = 0; iybin < nYBins; iybin++) {
      hphotonEnergy[iptbin][iybin]->SetLineColor(iybin + 1);
      hphotonEnergy[iptbin][iybin]->Draw("same");
      legphoton[iptbin]->AddEntry(hphotonEnergy[iptbin][iybin],
                                  Form("%.1f < y < %.1f, %.0f < p_{ T} < %.0f",
                                       yBins[iybin], yBins[iybin + 1],
                                       ptBins[iptbin], ptBins[iptbin + 1]),
                                  "l");
    }
    legphoton[iptbin]->Draw();
  }
  cPhotonproperties->SaveAs("cPhotonproperties.pdf");

  TCanvas *cGluonproperties =
      new TCanvas("cGluonproperties", "cGluonproperties", 1800, 600);
  cGluonproperties->Divide(3, 1);
  TLegend *leggluon[nPtBins];
  bool swithonplotting[nPtBins][nYBins] = {{true, true, true, true},
                                           {true, true, true, true},
                                           {true, true, true, true}};
  float xlow[nPtBins] = {0.00001, 0.00001, 0.00001};
  float xup[nPtBins] = {10., 10., 10.};
  float ymax[nPtBins] = {3., 3., 3.};
  float ymin[nPtBins] = {0.0001, 0.0001, 0.0001};
  TH2F *hempty[nPtBins];
  for (int iptbin = 0; iptbin < nPtBins; iptbin++) {
    cGluonproperties->cd(iptbin + 1);
    gPad->SetLogy();
    gPad->SetLogx();
    leggluon[iptbin] = new TLegend(0.3, 0.65, 0.8, 0.86);
    leggluon[iptbin]->SetTextSize(0.03);
    leggluon[iptbin]->SetTextFont(42);
    leggluon[iptbin]->SetBorderSize(0);
    hempty[iptbin] = new TH2F("hempty", "", 100, xlow[iptbin], xup[iptbin], 100,
                              ymin[iptbin], ymax[iptbin]);
    hempty[iptbin]->GetYaxis()->SetTitleOffset(1.3);
    hempty[iptbin]->GetYaxis()->SetTitle("Self normalized dN/dx_{Bj}");
    hempty[iptbin]->GetXaxis()->SetTitle("Gluon x_{Bj}");
    hempty[iptbin]->GetXaxis()->SetRangeUser(xlow[iptbin], xup[iptbin]);
    hempty[iptbin]->SetMinimum(ymin[iptbin]);
    hempty[iptbin]->SetMaximum(ymax[iptbin]);
    hempty[iptbin]->Draw();
    for (int iybin = 0; iybin < nYBins; iybin++) {
      hgluonxBjorken[iptbin][iybin]->SetLineColor(iybin + 1);
      if (!swithonplotting[iptbin][iybin])
        continue;
      hgluonxBjorken[iptbin][iybin]->Draw("same");
      leggluon[iptbin]->AddEntry(hgluonxBjorken[iptbin][iybin],
                                 Form("%.1f < y < %.1f, %.0f < p_{ T} < %.0f",
                                      yBins[iybin], yBins[iybin + 1],
                                      ptBins[iptbin], ptBins[iptbin + 1]),
                                 "l");
    }
    leggluon[iptbin]->Draw();
  }
  cGluonproperties->SaveAs("cGluonproperties.pdf");
  TCanvas *cGluonpropertiesQ2 =
      new TCanvas("cGluonpropertiesQ2", "cGluonpropertiesQ2", 1800, 600);
  cGluonpropertiesQ2->Divide(3, 1);
  TLegend *leggluonQ2[nPtBins];

  bool swithonplottingQ2[nPtBins][nYBins] = {{true, true, true, true},
                                             {true, true, true, true},
                                             {true, true, true, true}};
  float xlowQ2[nPtBins] = {1., 1., 1.};
  float xupQ2[nPtBins] = {1000., 1000., 1000.};
  float yminQ2[nPtBins] = {0.0001, 0.0001, 0.0001};
  float ymaxQ2[nPtBins] = {10., 10., 10.};
  TH2F *hemptyQ2[nPtBins];
  for (int iptbin = 0; iptbin < nPtBins; iptbin++) {
    cGluonpropertiesQ2->cd(iptbin + 1);
    gPad->SetLogy();
    gPad->SetLogx();
    leggluonQ2[iptbin] = new TLegend(0.3, 0.65, 0.8, 0.86);
    leggluonQ2[iptbin]->SetTextSize(0.03);
    leggluonQ2[iptbin]->SetTextFont(42);
    leggluonQ2[iptbin]->SetBorderSize(0);
    hemptyQ2[iptbin] =
        new TH2F("hemptyQ2", "", 100, xlowQ2[iptbin], xupQ2[iptbin], 100,
                 yminQ2[iptbin], ymaxQ2[iptbin]);
    hemptyQ2[iptbin]->GetXaxis()->SetRangeUser(xlowQ2[iptbin], xupQ2[iptbin]);
    hemptyQ2[iptbin]->GetYaxis()->SetTitleOffset(1.3);
    hemptyQ2[iptbin]->GetYaxis()->SetTitle("Self normalized dN/Q^{2}}");
    hemptyQ2[iptbin]->GetXaxis()->SetTitle("Gluon Q^{2}");
    hemptyQ2[iptbin]->SetMinimum(yminQ2[iptbin]);
    hemptyQ2[iptbin]->SetMaximum(ymaxQ2[iptbin]);
    hemptyQ2[iptbin]->Draw();
    for (int iybin = 0; iybin < nYBins; iybin++) {
      if (!swithonplottingQ2[iptbin][iybin])
        continue;
      hgluonQ2[iptbin][iybin]->SetLineColor(iybin + 1);
      hgluonQ2[iptbin][iybin]->Draw("same");
      leggluonQ2[iptbin]->AddEntry(hgluonQ2[iptbin][iybin],
                                   Form("%.1f < y < %.1f, %.0f < p_{ T} < %.0f",
                                        yBins[iybin], yBins[iybin + 1],
                                        ptBins[iptbin], ptBins[iptbin + 1]),
                                   "l");
    }
    leggluonQ2[iptbin]->Draw();
  }
  cGluonpropertiesQ2->SaveAs("cStudyCoverage.pdf");
}
